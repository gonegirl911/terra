#include "application.hpp"

Application::Application() : m_window{}, m_renderer{m_window}, m_triangle{m_renderer} {
  m_window.setUserData(*this);
  m_window.setResizeCallback([](auto window, auto, auto) {
    Window::getUserData<Application>(window).m_renderer.shouldResize = true;
  });
  m_window.setMouseButtonCallback([](auto window, auto key, auto action, auto) {
    Window::getUserData<Application>(window).m_window.onMouseButton(key, action);
  });
  m_window.setKeyCallback([](auto window, auto key, auto, auto action, auto) {
    Window::getUserData<Application>(window).m_window.onKey(key, action);
  });
}

void Application::run() {
  m_window.run([&] {
    update();
    draw();
  });
}

void Application::update() { m_renderer.update(m_window); }

void Application::draw() {
  m_renderer.draw([&](auto view, auto encoder) { m_triangle.draw(view, encoder); });
}
