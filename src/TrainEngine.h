//#pragma once
//
//#include <boost/noncopyable.hpp>
//#include <memory>
//
//namespace trm
//{
//	struct TrainPart;
//
//	class TrainEngine
//		: boost::noncopyable
//	{
//	public:
//		TrainEngine(const float dist);
//
//		void SetTrainLength(const float len);
//		void Append(const TrainPart & tp);
//
//		float GetMoveDistance() const;
//		bool UpdateMoveDistance();
//
//	private:
//		float distance_;
//		float passed_;
//		float moveStep_;
//		float speed_;
//		float maxSpeed_;
//		float acceleration_;
//		float breaking_;
//	};
//
//	typedef std::shared_ptr<TrainEngine> TrainEngineSPtr;
//	typedef std::weak_ptr<TrainEngine> TrainEngineWPtr;
//
//} // namespace trm
