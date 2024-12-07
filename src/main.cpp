#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 頂點著色器原始碼
static auto vertex_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n\0";

// 片段著色器原始碼
static auto fragment_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main()
{
    // 初始化 GLFW
    glfwInit();

    // 告訴 GLFW 使用的 OpenGL 版本
    // 這裡使用的是 OpenGL 3.3 版
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // 告訴 GLFW 使用 CORE profile
    // 這代表此程式只能使用現代的函式功能
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 創建 GLFWwindow 視窗物件，解析度為 800x800 像素，並命名為 "OpenGL Learning"
    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Learning", nullptr, nullptr);
    // 確認是否成功創建
    if (window == nullptr)
    {
        // 建立失敗則回傳錯誤代碼
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }
    // 將視窗設定為目前的 OpenGL context
    glfwMakeContextCurrent(window);

    // 載入 GLAD 函式庫並配置 OpenGL
    gladLoadGL();

    // 設定 OpenGL viewport，大小為 800x800
    glViewport(0, 0, 800, 800);

    // 建立頂點著色器物件 (Vertex)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 將頂點著色器原始碼附加到著色器物件中
    glShaderSource(vertexShader, 1, &vertex_shader_source, nullptr);
    // 編譯頂點著色器
    glCompileShader(vertexShader);

    // 建立片段著色器物件 (Shader)
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 將片段著色器原始碼附加到著色器物件中
    glShaderSource(fragmentShader, 1, &fragment_shader_source, nullptr);
    // 編譯片段著色器
    glCompileShader(fragmentShader);

    // 建立著色器程序物件 (Program)
    GLuint shaderProgram = glCreateProgram();
    // 將頂點器與片段著色附加到程序物件中
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // 連結著色器程序
    glLinkProgram(shaderProgram);

    // 刪除不再需要的著色器物件
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 定義三角形的定點資料
    GLfloat vertices[] =
    {
        -0.5f, -0.5f * static_cast<float>(sqrt(3)) / 3, 0.0f, // 第一個頂點 (左下)
        0.5f, -0.5f * static_cast<float>(sqrt(3)) / 3, 0.0f, // 第二個頂點 (右下)
        0.0f, 0.5f * static_cast<float>(sqrt(3)) * 2 / 3, 0.0f, // 第三個頂點 (上方)
        -0.5f / 2, 0.5f * static_cast<float>(sqrt(3)) / 6, 0.0f, // 第四個頂點 (內部左邊)
        0.5f / 2, 0.5f * static_cast<float>(sqrt(3)) / 6, 0.0f, // 第五個頂點 (內部右邊)
        -0.0f, -0.5f * static_cast<float>(sqrt(3)) / 3, 0.0f, // 第六個頂點 (內部下方)
    };

    GLuint indices[] =
    {
        0, 3, 5,
        3, 2, 4,
        5, 4, 1
    };

    // 建立頂點陣列物件 (Vertex Array Object, VAO) 與頂點緩衝物件 (Vertex Buffer Object, VBO)
    // 元件緩衝物件 (Element Buffer Object, EBO)
    // 各別只有一個物件
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 綁定 VAO 與 VBO (使用 GL_ARRAY_BUFFER)
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 將頂點資料複製到 VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 設定頂點屬性指標，告知 OpenGL 如何解析頂點數據
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
    // 啟用頂點屬性，讓 OpenGL 知道去使用
    glEnableVertexAttribArray(0);

    // 解除綁定 VBO 和 VAO，就不會不小心修改到以創建的 VAO 與 VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // 主迴圈
    while (!glfwWindowShouldClose(window))
    {
        // 設定背景顏色
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // 清除顏色緩衝區，並指派新顏色
        glClear(GL_COLOR_BUFFER_BIT);
        // 告訴 OpenGL 我們要使用的著色器程序
        glUseProgram(shaderProgram);
        // 綁定 VAO 讓 OpenGL 使用
        glBindVertexArray(VAO);
        // 繪製三角形，使用 GL_TRIANGLES 模式，從頂點 0 開始繪製 3 個頂點
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        // 交換前緩衝區，顯示選染結果
        glfwSwapBuffers(window);

        // 處理 GLFW 事件，例如鍵盤滑鼠的輸入
        glfwPollEvents();
    }

    // 刪除 VAO、VBO 與著色器程序
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // 刪除 GLFW 視窗
    glfwDestroyWindow(window);
    // 中斷 GLFW
    glfwTerminate();
    return 0;
}
