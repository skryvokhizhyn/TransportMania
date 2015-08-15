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

#ifdef UNIT_TESTING_ON

	public:	
		typedef std::pair<EdgeValueType, bool> EdgeValueSearchType;
		EdgeValueSearchType GetEdgeValue(const PointValueType p1, const PointValueType p2);
		bool CheckPointExists(const PointValueType p) const;

#endif // UNIT_TESTING_ON		

	public:
		bool InsertEdge(const PointValueType p1, const PointValueType p2, const EdgeValueType edge);
		bool RemoveEdge(const PointValueType p1, const PointValueType p2);
		Route GetRoute(const PointValueType p1, const PointValueType p2);

	private:
		typedef boost::property<boost::edge_weight_t, EdgeValueType> EdgeWeight;
		typedef boost::adjacency_list_traits<boost::setS, boost::listS, boost::undirectedS> GraphTraits;
		typedef GraphTraits::vertex_descriptor VertexType;
		typedef GraphTraits::edge_descriptor EdgeType;
		typedef boost::property<boost::vertex_index_t, int, boost::property<boost::vertex_predecessor_t, VertexType>> VertexProperties;
		typedef boost::adjacency_list<boost::setS, boost::listS, boost::undirectedS, VertexProperties, EdgeWeight> Graph;
		typedef boost::bimap<VertexType, PointValueType> VertexDescriptorMap;
		typedef std::pair<VertexType, bool> VertexSearchType;
		typedef std::tuple<VertexType, VertexType, bool> VertexesSearchType;
		typedef std::pair<EdgeType, bool> EdgeSearchType;
		using IndexMapType = typename boost::property_map<Graph, boost::vertex_index_t>::type;

	private:
		VertexType InsertPoint(const PointValueType point);
		VertexSearchType GetVertex(const PointValueType p1) const;
		VertexesSearchType GetVertexes(const PointValueType p1, const PointValueType p2) const;
		EdgeSearchType GetEdge(const PointValueType p1, const PointValueType p2);
		void TryRemoveVertex(const VertexType & v);
		void RebuildIndexMap();

	private:
		Graph graph_;
		VertexDescriptorMap vertexDescriptorMap_;
		bool indexMapInvalid_ = false;
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
	void RoadNetworkImpl<PointValueType, EdgeValueType>::TryRemoveVertex(const VertexType & v)
	{
		const auto e_out = boost::out_edges(v, graph_);
		const auto e_in = boost::in_edges(v, graph_);

		// if no edges exist then do proper removal
		if (e_out.first == e_out.second && e_in.first == e_in.second)
		{
			boost::remove_vertex(v, graph_);
			vertexDescriptorMap_.left.erase(v);
			indexMapInvalid_ = true;
			// no need to clean up index map as this property is removed with vertex
		}
	}

	template<typename PointValueType, typename EdgeValueType>
	bool RoadNetworkImpl<PointValueType, EdgeValueType>::RemoveEdge(const PointValueType p1, const PointValueType p2)
	{
		const VertexesSearchType v = GetVertexes(p1, p2);

		if (!std::get<2>(v))
			return false;

		const EdgeSearchType e = boost::edge(std::get<0>(v), std::get<1>(v), graph_);

		if (!e.second)
			return false;

		boost::remove_edge(e.first, graph_);

		TryRemoveVertex(std::get<0>(v));
		TryRemoveVertex(std::get<1>(v));

		return true;
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

			if (!indexMapInvalid_)
			{
				IndexMapType indexMap = boost::get(boost::vertex_index, graph_);
				indexMap[vertex] = boost::num_vertices(graph_) - 1;
			}
		}

		return vertex;
	}

	template<typename PointValueType, typename EdgeValueType>
	auto RoadNetworkImpl<PointValueType, EdgeValueType>::GetVertex(const PointValueType p) const -> VertexSearchType
	{
		const auto pI = vertexDescriptorMap_.right.find(p);
		if (pI == vertexDescriptorMap_.right.end())
		{
			return VertexSearchType(VertexType(), false);
		}
		
		return VertexSearchType(pI->second, true);
	}

	template<typename PointValueType, typename EdgeValueType>
	auto RoadNetworkImpl<PointValueType, EdgeValueType>::GetVertexes(const PointValueType p1, const PointValueType p2) const -> VertexesSearchType
	{
		const VertexesSearchType falseResult(VertexType(), VertexType(), false);

		const VertexSearchType v1S = GetVertex(p1);

		if (!v1S.second)
			return falseResult;

		const VertexSearchType v2S = GetVertex(p2);

		if (!v2S.second)
			return falseResult;

		return VertexesSearchType(v1S.first, v2S.first, true);
	}

#ifdef UNIT_TESTING_ON

	template<typename PointValueType, typename EdgeValueType>
	auto RoadNetworkImpl<PointValueType, EdgeValueType>::GetEdgeValue(const PointValueType p1, const PointValueType p2) -> EdgeValueSearchType
	{
		const auto e = GetEdge(p1, p2);

		if (!e.second)
		{
			return EdgeValueSearchType(EdgeValueType(), false);
		}

		using EdgeWeightPropertyType = typename boost::property_map<Graph, boost::edge_weight_t>::type;

		EdgeWeightPropertyType edgeWeightProperty = boost::get(boost::edge_weight_t(), graph_);
		return EdgeValueSearchType(edgeWeightProperty[e.first], true);
	}

	template<typename PointValueType, typename EdgeValueType>
	bool RoadNetworkImpl<PointValueType, EdgeValueType>::CheckPointExists(const PointValueType p) const
	{
		return GetVertex(p).second;
	}

#endif // UNIT_TESTING_ON

	template<typename PointValueType, typename EdgeValueType>
	auto RoadNetworkImpl<PointValueType, EdgeValueType>::GetEdge(const PointValueType p1, const PointValueType p2) -> EdgeSearchType
	{
		const VertexesSearchType vertexes = GetVertexes(p1, p2);

		if (std::get<2>(vertexes) == false)
		{
			return EdgeSearchType(EdgeType(), false);
		}

		return boost::edge(std::get<0>(vertexes), std::get<1>(vertexes), graph_);
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
	void RoadNetworkImpl<PointValueType, EdgeValueType>::RebuildIndexMap()
	{
		IndexMapType indexMap = boost::get(boost::vertex_index, graph_);

		typedef typename boost::graph_traits<Graph>::vertex_iterator VertexIter;
		VertexIter vIter, vIterEnd;
		int c = 0;
		
		for(boost::tie(vIter, vIterEnd) = boost::vertices(graph_); 
			vIter != vIterEnd; ++vIter, ++c)
		{
			indexMap[*vIter] = c;
		}

		indexMapInvalid_ = false;
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

		if (indexMapInvalid_)
			RebuildIndexMap();

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
