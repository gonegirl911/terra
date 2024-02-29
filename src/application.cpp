#include "application.hpp"
#include "window.hpp"

Application::Application()
    : m_window{},
      m_renderer{m_window},
      m_camera{m_renderer},
      m_terrain{m_renderer, m_camera.bindGroupLayout()} {
  m_window.setUserData(*this);

  m_window.setResizeCallback([](auto window, auto, auto) {
    Window::getUserData<Application>(window).m_renderer.shouldResize = true;
  });

  m_window.setMouseButtonCallback([](auto window, auto key, auto action, auto) {
    Window::getUserData<Application>(window).m_window.onMouseButton(key, action);
  });

  m_window.setKeyCallback([](auto window, auto key, auto, auto action, auto) {
    auto& application = Window::getUserData<Application>(window);
    application.m_window.onKey(key, action);
    application.m_camera.onKey(key, action);
  });

  m_window.setCursorPosCallback([](auto window, auto xpos, auto ypos) {
    auto& application = Window::getUserData<Application>(window);
    const auto [dx, dy] = application.m_window.onCursorPos(xpos, ypos);
    application.m_camera.onCursorPos(dx, dy);
  });
}

void Application::run() {
  m_window.run([&](auto dt) {
    update(dt);
    draw();
  });
}

void Application::update(float dt) {
  m_renderer.update(m_window);
  m_camera.update(m_renderer, dt);
}

void Application::draw() {
  // clang-format off
  m_renderer.draw([&](auto view, auto encoder) {
    m_terrain.draw(view, encoder, m_camera.bindGroup());
  });
  // clang-format on
}
