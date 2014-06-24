#pragma once

#pragma warning(push)
#pragma warning(disable: 4512 4100 4510 4610)

#include <boost/graph/adjacency_list.hpp>
#include <boost/noncopyable.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/bimap.hpp>
#include <algorithm>
#include <tuple>
#include <deque>

namespace trm
{
namespace network_impl
{
	template<typename PointValueType, typename EdgeValueType>
	class RoadNetworkImpl
		: boost::noncopyable
	{ 
	public:
		class Route
		{
		public:
			template<typename It>
			class Iterator
			{
			public:
				bool Next () {return ++it_ != end_;}
				typename It::value_type Value () const {return *it_;}

			private:
				friend class Route;
				Iterator(const It it, const It end) : it_(it), end_(end) {}

			private:
				It it_;
				const It end_;
			};

		private:
			typedef std::deque<PointValueType> RouteType;
		public:

			typedef Iterator<typename RouteType::const_iterator> RouteIterator;
			typedef Iterator<typename RouteType::const_reverse_iterator> RouteReverseIterator;

		public:
			void Insert(const PointValueType p);
			size_t Size() const;

			RouteIterator Begin() const;
			RouteReverseIterator RBegin() const;

		private:
			RouteType route_;
		};

	public:	
		typedef std::pair<EdgeValueType, bool> EdgeSearchType;

	public:
		bool InsertEdge(const PointValueType p1, const PointValueType p2, const EdgeValueType edge);
		EdgeSearchType GetEdge(const PointValueType p1, const PointValueType p2);
		Route GetRoute(const PointValueType p1, const PointValueType p2);

	private:
		typedef boost::property<boost::edge_weight_t, EdgeValueType> EdgeWeight;
		typedef boost::adjacency_list_traits<boost::setS, boost::listS, boost::undirectedS>::vertex_descriptor VertexType;
		typedef boost::property<boost::vertex_index_t, int, boost::property<boost::vertex_predecessor_t, VertexType>> VertexProperties;
		typedef boost::adjacency_list<boost::setS, boost::listS, boost::undirectedS, VertexProperties, EdgeWeight> Graph;
		typedef boost::bimap<VertexType, PointValueType> VertexDescriptorMap;
		typedef std::tuple<VertexType, VertexType, bool> VertexesSearchType;

	private:
		VertexType InsertPoint(const PointValueType point);
		VertexesSearchType GetVertexes(const PointValueType p1, const PointValueType p2) const;

	private:
		Graph graph_;
		VertexDescriptorMap vertexDescriptorMap_;
	};

	template<typename PointValueType, typename EdgeValueType>
	bool RoadNetworkImpl<PointValueType, EdgeValueType>::InsertEdge(const PointValueType p1, const PointValueType p2, const EdgeValueType edge)
	{
		const VertexType startVertex = InsertPoint(p1);
		const VertexType endVertex = InsertPoint(p2);

		const auto e = boost::add_edge(startVertex, endVertex, graph_);

		if (e.second)
		{
			using EdgeWeightPropertyType = typename boost::property_map<Graph, boost::edge_weight_t>::type;

			EdgeWeightPropertyType edgeWeightProperty = boost::get(boost::edge_weight_t(), graph_);
			edgeWeightProperty[e.first] = edge;
		}

		return e.second;
	}

	template<typename PointValueType, typename EdgeValueType>
	auto RoadNetworkImpl<PointValueType, EdgeValueType>::InsertPoint(const PointValueType p) -> VertexType
	{
		VertexType vertex;

		typename VertexDescriptorMap::right_const_iterator it;

		if ((it = vertexDescriptorMap_.right.find(p)) != vertexDescriptorMap_.right.end())
		{
			vertex = it->second;
		}
		else
		{
			vertex = boost::add_vertex(graph_);
			vertexDescriptorMap_.insert(typename VertexDescriptorMap::value_type(vertex, p));

			using IndexMapType = typename boost::property_map<Graph, boost::vertex_index_t>::type;

			IndexMapType indexMap = boost::get(boost::vertex_index, graph_);
			indexMap[vertex] = boost::num_vertices(graph_) - 1;
		}

		return vertex;
	}

	template<typename PointValueType, typename EdgeValueType>
	auto RoadNetworkImpl<PointValueType, EdgeValueType>::GetVertexes(const PointValueType p1, const PointValueType p2) const -> VertexesSearchType
	{
		VertexesSearchType falseResult(VertexType(), VertexType(), false);

		const auto pI1 = vertexDescriptorMap_.right.find(p1);
		if (pI1 == vertexDescriptorMap_.right.end())
		{
			return falseResult;
		}
		const VertexType v1 = pI1->second;

		const auto pI2 = vertexDescriptorMap_.right.find(p2);
		if (pI2 == vertexDescriptorMap_.right.end())
		{
			return falseResult;
		}
		const VertexType v2 = pI2->second;

		return VertexesSearchType(v1, v2, true);
	}

	template<typename PointValueType, typename EdgeValueType>
	auto RoadNetworkImpl<PointValueType, EdgeValueType>::GetEdge(const PointValueType p1, const PointValueType p2) -> EdgeSearchType
	{
		const EdgeSearchType falseSearchResult = EdgeSearchType(EdgeValueType(), false);

		const VertexesSearchType vertexes = GetVertexes(p1, p2);

		if (std::get<2>(vertexes) == false)
		{
			return falseSearchResult;
		}

		const auto e = boost::edge(std::get<0>(vertexes), std::get<1>(vertexes), graph_);

		if (!e.second)
		{
			return falseSearchResult;
		}

		using EdgeWeightPropertyType = typename boost::property_map<Graph, boost::edge_weight_t>::type;

		EdgeWeightPropertyType edgeWeightProperty = boost::get(boost::edge_weight_t(), graph_);
		return EdgeSearchType(edgeWeightProperty[e.first], true);
	}

	template<typename PointValueType, typename EdgeValueType>
	size_t RoadNetworkImpl<PointValueType, EdgeValueType>::Route::Size() const
	{
		return route_.size();
	}

	template<typename PointValueType, typename EdgeValueType>
	auto RoadNetworkImpl<PointValueType, EdgeValueType>::Route::Begin() const -> RouteIterator
	{
		return RouteIterator(route_.cbegin(), route_.cend());
	}

	template<typename PointValueType, typename EdgeValueType>
	auto RoadNetworkImpl<PointValueType, EdgeValueType>::Route::RBegin() const -> RouteReverseIterator
	{
		return RouteReverseIterator(route_.crbegin(), route_.crend());
	}

	template<typename PointValueType, typename EdgeValueType>
	void RoadNetworkImpl<PointValueType, EdgeValueType>::Route::Insert(const PointValueType v)
	{
		route_.push_front(v);
	}

	template<typename PointValueType, typename EdgeValueType>
	auto RoadNetworkImpl<PointValueType, EdgeValueType>::GetRoute(const PointValueType p1, const PointValueType p2) -> Route
	{
		const VertexesSearchType vertexes = GetVertexes(p1, p2);

		Route result;

		if (std::get<2>(vertexes) == false)
		{
			return result;
		}

		const VertexType endVertex = std::get<1>(vertexes);
		const VertexType startVertex = std::get<0>(vertexes);

		using VertexPredecessorType = typename boost::property_map<Graph, boost::vertex_predecessor_t>::type;

		VertexPredecessorType p = boost::get(boost::vertex_predecessor, graph_);
		boost::dijkstra_shortest_paths(graph_, startVertex, boost::predecessor_map(p));

		VertexType u = endVertex;

		for (VertexType v = p[u]; u != v; u = p[v])
		{
			result.Insert(vertexDescriptorMap_.left.at(u));
			v = u;
		}

		// if no path exists, return will be empty
		/*if (u != startVertex)
		{
			throw std::runtime_error("Invalid path returned from road graph");
		}*/

		return result;
	}

} // namespace network_impl
} // namespace trm

#pragma warning(pop)
