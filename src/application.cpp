#include "application.hpp"

Application::Application() : m_window{}, m_renderer{m_window} {
  m_window.setUserData(*this);
  m_window.onResize([](auto window, auto, auto) {
    Window::getUserData<Application>(window).m_renderer.shouldResize = true;
  });
}

void Application::run() {
  m_window.run([&] { m_renderer.update(m_window); });
}
