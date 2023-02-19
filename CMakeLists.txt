FetchContent_Declare(llhttp
  URL "https://github.com/nodejs/llhttp/archive/refs/tags/v6.0.5.tar.gz")  # Using version 6.0.5

FetchContent_MakeAvailable(llhttp)

target_link_libraries(${EXAMPLE_PROJECT_NAME} ${PROJECT_LIBRARIES} llhttp ${PROJECT_NAME})