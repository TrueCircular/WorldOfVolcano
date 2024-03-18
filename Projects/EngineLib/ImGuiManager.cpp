#include "pch.h"
#include "ImGuiManager.h"

ImGuiManager* ImGuiManager::_instance = nullptr;

ImGuiManager::ImGuiManager()
{
}

ImGuiManager::~ImGuiManager()
{
}

ImVec4 GetRandomColor() {
    float r = static_cast<float>(rand()) / RAND_MAX; // 빨강 (0.0에서 1.0)
    float g = static_cast<float>(rand()) / RAND_MAX; // 초록 (0.0에서 1.0)
    float b = static_cast<float>(rand()) / RAND_MAX; // 파랑 (0.0에서 1.0)
    return ImVec4(r, g, b, 1.0f); // 알파는 1.0로 설정 (투명도)
}

void ImGuiManager::Init()
{
    //ImGui Main
    {
        // Show the window
        ::ShowWindow(g_gameDesc.hWnd, SW_SHOWDEFAULT);
        ::UpdateWindow(g_gameDesc.hWnd);
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        //한글 폰트 추가 - add kor font
        io.Fonts->AddFontFromFileTTF("../../Resources/Font/LINESeedKR-Bd.ttf", 22.0f, NULL, io.Fonts->GetGlyphRangesKorean());
        //io.Fonts->AddFontFromFileTTF("../../Resources/Font/Warhaven_Bold.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(g_gameDesc.hWnd);
        ImGui_ImplDX11_Init(DEVICE().Get(), DC().Get());

        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
    }
}

void ImGuiManager::Update()
{
    //ImGui Main
    {
        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::BeginFrame();
    }

    //show_main_window
    if (show_main_window)
    {
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        //텍스쳐
        {
            ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            //ImGui::Begin("Texture Window", &show_main_window, ImGuiWindowFlags_NoDecoration);
            ImGui::Begin("Texture Window", &show_main_window, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMouseInputs);
            shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"mainTitle");
            ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
            ImGui::Image(textureID, ImVec2(displaySize.x, displaySize.y)); // 텍스쳐의 크기를 조정할 수 있습니다.
            ImGui::End();
        }
        //타이틀 텍스트
        {
            float windowSizeX = 800;
            float windowSizeY = 150;
            // Set the window size to a fixed value
            ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(100, 50), ImGuiCond_Always);
            float r = 200.0f / 255.0f;
            float g = 15.0f / 255.0f;
            float b = 15.0f / 255.0f;
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.0f));
            ImGui::Begin("TitleWindow", &show_main_window, ImGuiWindowFlags_NoDecoration);
            
            ImGui::SetWindowFontScale(5.0f);
            {
                wchar_t buffer[256] = L"World of VVocano";
                int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                char* charBuffer = new char[bufferSize];
                WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                ImGui::TextColored(ImVec4(r, g, b, 1.0f), charBuffer);
                delete[] charBuffer;
            }
            ImGui::End();
            ImGui::PopStyleColor();
        }
        //UI버튼
        {
            float windowSizeX = 500;
            float windowSizeY = 500;
            // Set the window size to a fixed value
            ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);

            //ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2 - windowSizeX / 2, displaySize.y / 2 - windowSizeY / 2), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(50, displaySize.y - ImGui::GetFrameHeightWithSpacing() - windowSizeY + (windowSizeY / 8.0f)), ImGuiCond_Always);

            float r = 230.0f / 255.0f;
            float g = 90.0f / 255.0f;
            float b = 26.0f / 255.0f;
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.0f));
            ImGui::Begin("MainWindow", &show_main_window, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

            {
                ImVec2 buttonSize(400.0f, 80.0f);
                float r = 39.0f / 255.0f;
                float g = 52.0f / 255.0f;
                float b = 48.0f / 255.0f;
                ImVec4 buttonColor(r, g, b, 1.0f);
                ImGui::SetWindowFontScale(1.5f);

                {
                    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
                    wchar_t buffer[256] = L"게임시작";
                    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                    char* charBuffer = new char[bufferSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                    if (ImGui::Button(charBuffer, buttonSize))
                    {
                        _isGameStart = true;
                        show_main_window = false;
                        show_all_window = true;
                    }
                    delete[] charBuffer;
                    ImGui::PopStyleColor();
                }
                ImGui::NewLine();
                {
                    wchar_t buffer[256] = L"옵션";
                    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                    char* charBuffer = new char[bufferSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);

                    shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"mainTitle");
                    ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());

                    //if (ImGui::ImageButton(textureID, buttonSize))
                    if (ImGui::Button(charBuffer, buttonSize))
                    {
                        // 그런건없다.
                    }
                    delete[] charBuffer;
                    ImGui::PopStyleColor();
                }
                ImGui::NewLine();
                {
                    wchar_t buffer[256] = L"종료";
                    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                    char* charBuffer = new char[bufferSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
                    if (ImGui::Button(charBuffer, buttonSize))
                    {
                        std::quick_exit(0);
                    }
                    delete[] charBuffer;
                    ImGui::PopStyleColor();
                }
            }
            ImGui::PopStyleColor();
            ImGui::End();
        } 
    }

    //show_all_window
    if (show_all_window)
    {
        // show_hp_window
        if (show_hp_window)
        {
            // Set the window size to a fixed value
            ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_Always);
            // Set the window position to the top-left corner
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

            // 윈도우의 배경색을 투명도(alpha)에 따라 설정
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

            ImGui::Begin("Hp Window", &show_hp_window, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

            ImGui::PopStyleColor(3); // Push한 스타일을 복원

            ImGui::Text("Warrior");
            //Hp Bar
            {
                ImGui::Text("HP");
                float r = 187.0f / 255.0f;
                float g = 0.0f / 255.0f;
                float b = 2.0f / 255.0f;
                ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(r, g, b, 1.0f));
                ImGui::SameLine();
                ImGui::ProgressBar(_hp);
                ImGui::PopStyleColor(1);
            }
            //Mp Bar
            {
                ImGui::Text("MP");
                float r = 2.0f / 255.0f;
                float g = 30.0f / 255.0f;
                float b = 200.0f / 255.0f;
                ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(r, g, b, 1.0f));
                ImGui::SameLine();
                ImGui::ProgressBar(_mp);
                ImGui::PopStyleColor(1);
            }


            ImGui::End();
        }

        //show_picked_hp_window
        if (show_picked_hp_window)
        {
            float sizeX = 500.0f;
            float sizeY = 150.0f;
            ImGui::SetNextWindowSize(ImVec2(sizeX, sizeY), ImGuiCond_Always);
            ImVec2 displaySize = ImGui::GetIO().DisplaySize;
            ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2 - sizeX / 2, 0), ImGuiCond_Always);

            // 윈도우의 배경색을 투명도(alpha)에 따라 설정
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

            ImGui::Begin("Picked Hp Window", &show_picked_hp_window, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

            ImGui::PopStyleColor(3); // Push한 스타일을 복원

            ImVec2 textSize = ImGui::CalcTextSize(_name);
            float textPosX = (sizeX - textSize.x) * 0.5f;
            ImGui::SetCursorPosX(textPosX);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::Text(_name);
            ImGui::PopStyleColor();
            //Hp Bar
            {
                ImGui::Text("HP");
                float r = 187.0f / 255.0f;
                float g = 0.0f / 255.0f;
                float b = 2.0f / 255.0f;
                ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(r, g, b, 1.0f));
                ImGui::SameLine();
                ImGui::ProgressBar(_pickedHp);
                ImGui::PopStyleColor(1);
            }

            ImGui::End();
        }

        // show_chat_window
        if (show_chat_window)
        {
            float windowSizeX = 400.0f;
            float windowSizeY = 180.0f;
            // Set the window size to a fixed value
            ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
            ImVec2 displaySize = ImGui::GetIO().DisplaySize;
            ImGui::SetNextWindowPos(ImVec2(5.0f, displaySize.y - ImGui::GetFrameHeightWithSpacing() - windowSizeY + (windowSizeY / 8.0f)), ImGuiCond_Always);
            float r = 25.0f / 255.0f;
            float g = 25.0f / 255.0f;
            float b = 25.0f / 255.0f;
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.5f));
            ImGui::Begin("Chat", &show_chat_window, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
            ImGui::PopStyleColor(1); // Push한 스타일을 복원

            // 채팅 메시지 출력 리스트
            ImGui::BeginChild("ChatMessages", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
            /*for (const auto& message : chatMessages)
            {
                ImGui::TextWrapped("%s", message.c_str());
            }*/

            for (const auto& message : otherChatMessages)
            {
                ImGui::TextWrapped("%s", message.c_str());
            }

            ImGui::EndChild();

            // 채팅 입력박스
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
            if (ImGui::InputText("##ChatInput", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                // Enter 키를 누르면 "Send" 버튼과 동일한 동작 수행
                chatMessages.push_back(buffer);
                buffer[0] = '\0'; // 입력박스 초기화
                isScrollBottom = true;
            }
            ImGui::PopStyleColor(1);

            //채팅 입력버튼
            ImGui::SameLine();
            r = 208 / 255.0f;
            g = 171 / 255.0f;
            b = 156 / 255.0f;
            ImVec4 buttonColor(r, g, b, 0.5f);
            ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
            if (ImGui::Button("Send") && buffer[0] != '\0') // 버튼을 클릭하면 메시지를 보냄 (비어있지 않은 경우)
            {
                chatMessages.push_back(buffer);
                buffer[0] = '\0'; // 입력박스 초기화
                isScrollBottom = true;
            }
            ImGui::PopStyleColor();

            ImGui::SetScrollHereY(1.0f); // 스크롤을 가장 아래로 내리기
            ImGui::End();
        }

        // show_death_window
        if (show_death_window)
        {
            //TODO
            float windowSizeX = 300.0f;
            float windowSizeY = 150.0f;
            // Set the window size to a fixed value
            ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
            ImVec2 displaySize = ImGui::GetIO().DisplaySize;
            ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2 - windowSizeX / 2, displaySize.y / 2 - windowSizeY / 2), ImGuiCond_Always);
            float r = 25.0f / 255.0f;
            float g = 25.0f / 255.0f;
            float b = 25.0f / 255.0f;
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.9f));
            ImGui::Begin("DeathWindow", &show_death_window, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

            srand(static_cast<unsigned int>(time(nullptr))); // 난수 초기화
            ImVec4 dynamicColor = GetRandomColor();
            ImGui::NewLine();

            {
                wchar_t buffer[256] = L"                당신은 사망하였습니다.";
                int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                char* charBuffer = new char[bufferSize];
                WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                ImGui::TextColored(dynamicColor, charBuffer);
                delete[] charBuffer;
            }

            {
                wchar_t buffer[256] = L"                부활하시겠습니까?";
                int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                char* charBuffer = new char[bufferSize];
                WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                ImGui::TextColored(dynamicColor, charBuffer);
                delete[] charBuffer;
            }
            ImGui::NewLine();
            ImGui::NewLine();
            {
                ImVec2 buttonSize(100.0f, 30.0f);
                float r = 208 / 255.0f;
                float g = 171 / 255.0f;
                float b = 156 / 255.0f;
                ImVec4 buttonColor(r, g, b, 0.5f);
                {
                    ImGui::SameLine((ImGui::GetWindowWidth() / 2 - buttonSize.x / 2) * 0.5f);
                    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
                    wchar_t buffer[256] = L"예";
                    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                    char* charBuffer = new char[bufferSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                    if (ImGui::Button(charBuffer, buttonSize))
                    {
                        //TODO
                        for (int i = 0; i < 10; ++i)
                        {
                            _rebirthQueue.push(true);
                        }

                        show_death_window = false;
                    }
                    delete[] charBuffer;
                    ImGui::PopStyleColor();
                }
                ImGui::SameLine();
                {
                    wchar_t buffer[256] = L"아니오";
                    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                    char* charBuffer = new char[bufferSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
                    if (ImGui::Button(charBuffer, buttonSize))
                    {
                        // 그런건없다.
                    }
                    delete[] charBuffer;
                    ImGui::PopStyleColor();
                }
            }
            ImGui::PopStyleColor();
            ImGui::End();
        }

        // show_skill_window
        if (show_skill_window)
        {
            ImVec2 displaySize = ImGui::GetIO().DisplaySize;
            float windowSizeX = 64.0f;
            float windowSizeY = 64.0f;
            float windowPosX = displaySize.x / 2 - 100.0f;
            float windowPosY = displaySize.y - windowSizeY - 10.0f;
            float spacing = windowSizeX;

            float r = 232.0f / 255.0f;
            float g = 196.0f / 255.0f;
            float b = 184.0f / 255.0f;

            //box
            {
                ImGui::SetNextWindowSize(ImVec2(windowSizeX + 200, windowSizeY + 15), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(windowPosX, windowPosY), ImGuiCond_Always);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.8f));
                ImGui::Begin("box Window", &show_skill_window, ImGuiWindowFlags_NoDecoration);
                ImGui::End();
                ImGui::PopStyleColor();
            }
            //Skill1
            {
                ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(windowPosX, windowPosY), ImGuiCond_Always);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::Begin("Skill1 Window", &show_skill_window, ImGuiWindowFlags_NoDecoration);
                shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"skill_charge");
                Vec2 texSize = tex->GetSize();
                ImVec2 imageSize = ImVec2(windowSizeX, windowSizeY);
                if (texSize.x > imageSize.x || texSize.y > imageSize.y) {
                    float scale = min(imageSize.x / texSize.x, imageSize.y / texSize.y);
                    imageSize = ImVec2(texSize.x * scale, texSize.y * scale);
                }
                ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
                ImGui::Image(textureID, imageSize);
                ImGui::End();
                ImGui::PopStyleColor();
            }
            //Skill2
            {
                ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(windowPosX + spacing, windowPosY), ImGuiCond_Always);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::Begin("Skill2 Window", &show_skill_window, ImGuiWindowFlags_NoDecoration);
                shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"skill_shouting");
                ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
                ImGui::Image(textureID, ImVec2(windowSizeX, windowSizeY));
                ImGui::End();
                ImGui::PopStyleColor();
            }
            //Skill3
            {
                ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(windowPosX + spacing * 2, windowPosY), ImGuiCond_Always);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::Begin("Skill3 Window", &show_skill_window, ImGuiWindowFlags_NoDecoration);
                shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"HealthPotion");
                ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
                ImGui::Image(textureID, ImVec2(windowSizeX, windowSizeY));
                ImGui::End();
                ImGui::PopStyleColor();
            }
            //Skill4
            {
                ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(windowPosX + spacing * 3, windowPosY), ImGuiCond_Always);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::Begin("Skill4 Window", &show_skill_window, ImGuiWindowFlags_NoDecoration);
                shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"ManaPotion");
                ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
                ImGui::Image(textureID, ImVec2(windowSizeX, windowSizeY));
                ImGui::End();
                ImGui::PopStyleColor();
            }
        }
    }
}

void ImGuiManager::Render()
{
    //ImGui Main
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

void ImGuiManager::AddMessage(char message[50])
{
    // 받은 문자열을 std::string으로 변환
    std::string newMessage(message);

    // chatMessages 벡터에 추가
    otherChatMessages.push_back(newMessage);
}

void ImGuiManager::UpdateHp(uint32 maxHp, uint32 hp)
{
    _hp = static_cast<float>(hp) / static_cast<float>(maxHp);
}

void ImGuiManager::UpdateMp(uint32 maxMp, uint32 mp)
{
    _mp = static_cast<float>(mp) / static_cast<float>(maxMp);
}

void ImGuiManager::UpdatePicked(bool isPicked, uint32 maxHp, uint32 hp, wstring name)
{
    show_picked_hp_window = isPicked;
    _pickedHp = static_cast<float>(hp) / static_cast<float>(maxHp);

    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, name.c_str(), -1, nullptr, 0, nullptr, nullptr);
    _name = new char[bufferSize];
    WideCharToMultiByte(CP_UTF8, 0, name.c_str(), -1, _name, bufferSize, nullptr, nullptr);
}

int ImGuiManager::GetAttackQueueSize()
{
    if (_rebirthQueue.empty() == false)
    {
        int queueSize = _rebirthQueue.size();
        _rebirthQueue.pop();
        return queueSize;
    }

    return -1;
}
