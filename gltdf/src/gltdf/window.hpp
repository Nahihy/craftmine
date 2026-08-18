#pragma once

#include <GLFW/glfw3.h>

#include <functional>
#include <iostream>

namespace gltdf {

  class Window {

    private:
      float lastX;
      float lastY;

      bool firstMouse;

      float lastFrame;

    public:
      float deltaTime;
      GLFWwindow* glfwWindow;
      std::string windowTitle;
      int height, width;
      std::function<void(Window*, double, double)> mouseCallbackFunc;
      std::function<void(Window*, double, double)> scrollCallbackFunc;
      void* userPtr;

      Window(std::string windowTitle = "", int height = 600, int width = 800) 
      : windowTitle(windowTitle), height(height), width(width), lastX(width / 2), 
        lastY(height / 2), firstMouse(true), deltaTime(0.0f), lastFrame(0.0f) {
        this->glfwWindow = glfwCreateWindow(this->width, this->height, this->windowTitle.c_str(), nullptr, nullptr);
        if(this->glfwWindow == nullptr)
          std::cout << "Failed to create GLFW window: " << this->windowTitle << std::endl;
      }

      ~Window() {
        glfwDestroyWindow(this->glfwWindow);
        this->glfwWindow = nullptr;
      }

      void makeCurrent() {
        glfwMakeContextCurrent(this->glfwWindow);
      }

      bool shouldClose() {
        return glfwWindowShouldClose(this->glfwWindow);
      }

      void swapAndPollEvents() {
        glfwSwapBuffers(this->glfwWindow);
        glfwPollEvents();
      }

      void setCustomUserPtr(void* ptr) {
        this->userPtr = ptr;
      }

      void setAutoResizeFrameBuffer() {
        glfwSetWindowUserPointer(this->glfwWindow, this);

        glfwSetFramebufferSizeCallback(this->glfwWindow, Window::frameBufferSize_callback);
      }

      void setAutoResizeFrameBufferSAVERATIO() {
        glfwSetWindowUserPointer(this->glfwWindow, this);

        glfwSetFramebufferSizeCallback(this->glfwWindow, Window::frameBufferSizeSaveRatio_callback);
      }

      void setAutoOffsetCalcMouseCallback(std::function<void(Window*, double, double)> mouseCallbackFunc) {
        this->mouseCallbackFunc = mouseCallbackFunc;
        
        glfwSetCursorPosCallback(this->glfwWindow, Window::mouse_callback);
      }

      void setScollOffsetCallback(std::function<void(Window*, double, double)> scrollCallbackFunc) {
        this->scrollCallbackFunc = scrollCallbackFunc;
        
        glfwSetScrollCallback(this->glfwWindow, Window::scroll_callback);
      }   

      static void frameBufferSize_callback(GLFWwindow* window, int width, int height) {
        Window* windowClass = (Window*)glfwGetWindowUserPointer(window);

        glViewport(0, 0, width, height);
        windowClass->width = width;
        windowClass->height = height;
      }


      static void frameBufferSizeSaveRatio_callback(GLFWwindow* window, int width, int height) {
        Window* windowClass = (Window*)glfwGetWindowUserPointer(window);
        float window_aspect = (float)width / (float)height;
        
        int render_width, render_height;
        int offset_x = 0, offset_y = 0;
        
        if (window_aspect > ((float)windowClass->width / (float)windowClass->height)) {
            render_height = height;
          render_width = (int)(height * ((float)windowClass->width / (float)windowClass->height));
            offset_x = (width - render_width) / 2;
        } else {
            render_width = width;
          render_height = (int)(width / ((float)windowClass->width / (float)windowClass->height));
            offset_y = (height - render_height) / 2;
        }

        glViewport(offset_x, offset_y, render_width, render_height);

      };

      static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
        Window* windowClass = (Window*)glfwGetWindowUserPointer(window);
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (windowClass->firstMouse) {
          windowClass->lastX = xpos;
          windowClass->lastY = ypos;
          windowClass->firstMouse = false;
        }

        float xoffset = xpos - windowClass->lastX;
        float yoffset = windowClass->lastY - ypos;

        windowClass->lastX = xpos;
        windowClass->lastY = ypos;
        
        windowClass->mouseCallbackFunc(windowClass, xoffset, yoffset);
      }

      static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        Window* windowClass = (Window*)glfwGetWindowUserPointer(window);
        windowClass->scrollCallbackFunc(windowClass, xoffset, yoffset);
      }

      void updateDeltaTime() {
        float currentFrame = glfwGetTime();
        this->deltaTime = currentFrame - this->lastFrame;
        this->lastFrame = currentFrame;
      }

  };

}
