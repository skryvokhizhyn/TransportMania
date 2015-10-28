#include "WindowManager.h"
#include "EventContainer.h"
#include "WindowPosition.h"
#include "TextRendererProxy.h"

#include "WindowCloseEvent.h"
#include "ApplicationPauseEvent.h"
#include "MouseModeChangeEvent.h"

#include "WindowItemBox.h"
#include "WindowItemRenderer.h"
#include "WindowItemText.h"

#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/adaptor/map.hpp>

using namespace trm;

namespace
{
	const float OK_WINDOW_RELATIVE_SIZE = 0.2f;

	WindowItemBoxPtr CreatePauseGoButton(TextureId tId, const float szF)
	{
		Size2f sz(szF, szF);

		const UniqueId winId = UniqueId::Generate();

		WindowItemBoxPtr windowPtr(new WindowItemBox(winId, sz, tId,
			EventContainer::Create({ApplicationPauseEvent(), WindowCloseEvent(winId)}, EventActionType::Single), 
			WindowPosition::p100, WindowPosition::p100));

		return windowPtr;
	}

	WindowItemBoxPtr CreateMouseModeButton(TextureId tId, MoveSceneEventHandlerType nextType, const float szF)
	{
		Size2f sz(szF, szF);

		UniqueId winId = UniqueId::Generate();

		WindowItemBoxPtr windowPtr(new WindowItemBox(winId, sz, tId,
			EventContainer::Create({MouseModeChangeEvent(nextType), WindowCloseEvent(winId)}, EventActionType::Single),
			WindowPosition::p100, WindowPosition::p50));

		return windowPtr;
	}
}

void
WindowManager::Init(const Size2d & sz)
{
	screenSize_ = sz;
}

void 
WindowManager::ProcessWindowData(WindowItemPtr ptr)
{
	Size2f sz = Size2f::Cast(screenSize_);
	WindowItemRenderer renderer(Rectangle2f{Point2d(), sz.x(), sz.y()});
	ptr->Accept(renderer);
	
	WindowImpl impl(renderer.GetData());

	windows_.emplace(impl.Id(), impl);
}

void
WindowManager::CreateOKButton(EventAction cb)
{
	const float windowSize = screenSize_.y() * OK_WINDOW_RELATIVE_SIZE;

	Size2f sz(windowSize, windowSize);
	UniqueId windowId = UniqueId::Generate();

	WindowItemBoxPtr windowPtr(new WindowItemBox(windowId, sz, TextureId::OkButton,
		EventContainer::Create({cb, WindowCloseEvent(windowId)}, EventActionType::Single), 
		WindowPosition::p100, WindowPosition::p0));

	ProcessWindowData(std::move(windowPtr));
}

void 
WindowManager::CreateNOButton(EventAction cb)
{
	const float windowSize = screenSize_.y() * OK_WINDOW_RELATIVE_SIZE;

	Size2f sz(windowSize, windowSize);
	UniqueId windowId = UniqueId::Generate();

	WindowItemBoxPtr windowPtr(new WindowItemBox(windowId, sz, TextureId::NoButton,
		EventContainer::Create({cb, WindowCloseEvent(windowId)}, EventActionType::Single), 
		WindowPosition::p75, WindowPosition::p0));

	ProcessWindowData(std::move(windowPtr));
}

void 
WindowManager::CreateOKNOGroup(EventAction cbOK, EventAction cbNO)
{
	UniqueId windowOKId = UniqueId::Generate();
	UniqueId windowNOId = UniqueId::Generate();

	const float windowSize = screenSize_.y() * OK_WINDOW_RELATIVE_SIZE;
	Size2f sz(windowSize, windowSize);

	WindowItemBoxPtr windowOKPtr(new WindowItemBox(windowOKId, sz, TextureId::OkButton,
		EventContainer::Create({cbOK, WindowCloseEvent(windowOKId), WindowCloseEvent(windowNOId)}, EventActionType::Single), 
		WindowPosition::p100, WindowPosition::p0));

	WindowItemBoxPtr windowNOPtr(new WindowItemBox(windowNOId, sz, TextureId::NoButton,
		EventContainer::Create({cbNO, WindowCloseEvent(windowNOId), WindowCloseEvent(windowOKId)}, EventActionType::Single), 
		WindowPosition::p75, WindowPosition::p0));

	ProcessWindowData(std::move(windowOKPtr));
	ProcessWindowData(std::move(windowNOPtr));
}

void
WindowManager::CreatePauseButton()
{
	const float szF = screenSize_.y() * OK_WINDOW_RELATIVE_SIZE;

	ProcessWindowData(std::move(
		CreatePauseGoButton(TextureId::PauseButton, szF)));
}

void 
WindowManager::CreateGoButton()
{
	const float szF = screenSize_.y() * OK_WINDOW_RELATIVE_SIZE;

	ProcessWindowData(std::move(
		CreatePauseGoButton(TextureId::GoButton, szF)));
}

void 
WindowManager::CreateLockScreen()
{
	Size2f sz = Size2f::Cast(screenSize_);

	WindowItemBoxPtr windowPtr(new WindowItemBox(UniqueId::Generate(), sz, TextureId::TransparentGray,
		EventContainer::Create({}, EventActionType::Repeatable), 
		WindowPosition::p0, WindowPosition::p0));

	ProcessWindowData(std::move(windowPtr));
}

void 
WindowManager::CreateTextWindow(const std::wstring & text)
{
	const float TEXT_WINDOW_SIZE = 0.3f; // 30% from screen sizes
	Size2f windowSize = Size2f::Cast(screenSize_);
	windowSize *= TEXT_WINDOW_SIZE;

	UniqueId windowId = UniqueId::Generate();

	WindowItemBoxPtr windowPtr(new WindowItemBox(windowId, windowSize, TextureId::WindowBox,
		EventContainer::Create({WindowCloseEvent(windowId)}, EventActionType::Single), 
		WindowPosition::p50, WindowPosition::p50));

	const float FONT_SIZE = 0.05f; // 5% from screen height

	WindowItemPtr textPtr(new WindowItemText(text, screenSize_.y() * FONT_SIZE,
		WindowPosition::p50, WindowPosition::p50));

	windowPtr->Add(std::move(textPtr));

	ProcessWindowData(std::move(windowPtr));
}

void 
WindowManager::CloseWindow(UniqueId id)
{
	auto it = windows_.find(id);
	if (it == windows_.end())
	{
		throw std::runtime_error((boost::format("Trying to remove unknown window id=%d") % id).str());
	}

	it->second.Close();
	windows_.erase(it);
}

void 
WindowManager::CreateSceneMoveButton()
{
	const float windowSize = screenSize_.y() * OK_WINDOW_RELATIVE_SIZE;

	/*Size2f sz(windowSize, windowSize);

	UniqueId winId = UniqueId::Generate();

	WindowItemBoxPtr windowPtr(new WindowItemBox(winId, sz, TextureId::MouseMode,
		EventContainer::Create({MouseModeChangeEvent(MoveSceneEventHandlerType::Draw), WindowCloseEvent(winId)}, EventActionType::Single), 
		WindowPosition::p100, WindowPosition::p50));*/

	//ProcessWindowData(std::move(windowPtr));

	ProcessWindowData(CreateMouseModeButton(TextureId::SceneMove, MoveSceneEventHandlerType::Draw, windowSize));
}

void 
WindowManager::CreateRoadDrawButton()
{
	const float windowSize = screenSize_.y() * OK_WINDOW_RELATIVE_SIZE;

	/*Size2f sz(windowSize, windowSize);

	UniqueId winId = UniqueId::Generate();

	WindowItemBoxPtr windowPtr(new WindowItemBox(winId, sz, TextureId::MouseMode,
		EventContainer::Create({MouseModeChangeEvent(MoveSceneEventHandlerType::Draw), WindowCloseEvent(winId)}, EventActionType::Single), 
		WindowPosition::p100, WindowPosition::p50));

	ProcessWindowData(std::move(windowPtr));*/

	ProcessWindowData(CreateMouseModeButton(TextureId::RoadDraw, MoveSceneEventHandlerType::Move, windowSize));
}

void
WindowManager::Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const
{
	boost::for_each(windows_ | boost::adaptors::map_values, 
		boost::bind(&WindowImpl::Draw, _1, boost::cref(c), boost::cref(orthoViewMatrix)));
}
