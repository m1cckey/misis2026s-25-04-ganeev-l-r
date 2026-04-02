cmake_minimum_required(VERSION 3.20)
project(misis2026s-25-00-polevoi-d-v)

set(CMAKE_CXX_STANDARD 20)

# Подключаем labs
if(EXISTS prj.labs)
  add_subdirectory(prj.labs)
endif()

# Тесты
if(EXISTS prj.test)
  add_subdirectory(prj.test)
endif()
