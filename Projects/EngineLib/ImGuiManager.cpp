#include "pch.h"
#include "ImGuiManager.h"
#include "CharacterInfo.h"
#include "AbilitySlot.h"
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
    //Script-
    {
        scripts.push_back(L"이보시오 제가 보이는겁니까?\n제 이름은 마그니 브론즈 베어드라고 합니다.\n이곳은 오래전에 강림한 라그나로스에 의해\n모든게 황폐화가 된 상태입니다..\n보시는 것처럼 저같은 영령만 떠돌아 다닐 뿐\n이미 인적은 끊긴지 오래지요");
        scripts.push_back(L"당신같은 강한 용사만을 기다려왔다카우...");
        scripts.push_back(L"부디 화산심장부에 들어가\n남작 게돈을 물리쳐주십시오.\n그 뒤 화산심장부 깊은 곳 라그나로스가\n봉인된 포탈 관문 앞에서 기다리겠습니다.");
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
            ImGui::SetNextWindowSize(ImVec2(displaySize.x + 100.0f, displaySize.y + 100.0f), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(-10.0f, -10.0f), ImGuiCond_Always);
            //ImGui::Begin("Texture Window", &show_main_window, ImGuiWindowFlags_NoDecoration);
            ImGui::Begin("Texture Window", &show_main_window, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMouseInputs);
            shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"mainTitle");
            ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
            ImGui::Image(textureID, ImVec2(displaySize.x, displaySize.y)); // 텍스쳐의 크기를 조정할 수 있습니다.
            ImGui::End();
        }
        //텍스쳐
        {
            ImVec2 windowSize = ImVec2(1000.0f, 250.0f);
            ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.0f));
            ImGui::Begin("logo Window", &show_main_window, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMouseInputs);
            shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"logo");
            ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
            ImGui::Image(textureID, windowSize); // 텍스쳐의 크기를 조정할 수 있습니다.
            ImGui::End();
            ImGui::PopStyleColor();
        }
        //타이틀 텍스트
        {
            //float windowSizeX = 800;
            //float windowSizeY = 150;
            //// Set the window size to a fixed value
            //ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
            //ImGui::SetNextWindowPos(ImVec2(100, 50), ImGuiCond_Always);
            //float r = 200.0f / 255.0f;
            //float g = 15.0f / 255.0f;
            //float b = 15.0f / 255.0f;
            //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.0f));
            //ImGui::Begin("TitleWindow", &show_main_window, ImGuiWindowFlags_NoDecoration);
            //
            //ImGui::SetWindowFontScale(5.0f);
            //{
            //    wchar_t buffer[256] = L"World of VVocano";
            //    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
            //    char* charBuffer = new char[bufferSize];
            //    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
            //    ImGui::TextColored(ImVec4(r, g, b, 1.0f), charBuffer);
            //    delete[] charBuffer;
            //}
            //ImGui::End();
            //ImGui::PopStyleColor();
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
                float hr = 29.0f / 255.0f;
                float hg = 42.0f / 255.0f;
                float hb = 38.0f / 255.0f;
                ImVec4 buttonHoveredColor = ImVec4(hr, hg, hb, 1.0f);
                ImGui::SetWindowFontScale(1.5f);

                {
                    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoveredColor);
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
                    ImGui::PopStyleColor(2);
                }
                //ImGui::NewLine();
                //{
                //    wchar_t buffer[256] = L"옵션";
                //    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                //    char* charBuffer = new char[bufferSize];
                //    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                //    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
                //    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoveredColor);

                //    shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"mainTitle");
                //    ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());

                //    //if (ImGui::ImageButton(textureID, buttonSize))
                //    if (ImGui::Button(charBuffer, buttonSize))
                //    {
                //        // 그런건없다.
                //    }
                //    delete[] charBuffer;
                //    ImGui::PopStyleColor(2);
                //}
                ImGui::NewLine();
                {
                    wchar_t buffer[256] = L"종료";
                    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                    char* charBuffer = new char[bufferSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHoveredColor);
                    if (ImGui::Button(charBuffer, buttonSize))
                    {
                        std::quick_exit(0);
                    }
                    delete[] charBuffer;
                    ImGui::PopStyleColor(2);
                }
            }
            ImGui::PopStyleColor();
            ImGui::End();
        } 
    }

    //show_ending_window
    if (show_ending_window)
    {
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        //텍스쳐
        {
            ImGui::SetNextWindowSize(ImVec2(displaySize.x + 100.0f, displaySize.y + 100.0f), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(-10.0f, -10.0f), ImGuiCond_Always);
            //ImGui::Begin("Texture Window", &show_main_window, ImGuiWindowFlags_NoDecoration);
            ImGui::Begin("ending Window", &show_main_window, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMouseInputs);
            shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"victory");
            ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
            ImGui::Image(textureID, ImVec2(displaySize.x, displaySize.y)); // 텍스쳐의 크기를 조정할 수 있습니다.
            ImGui::End();
        }
        //텍스쳐
        {
            ImVec2 windowSize = ImVec2(1000.0f, 250.0f);
            ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(displaySize.x / 2 - windowSize.x / 2, displaySize.y / 2), ImGuiCond_Always);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.0f));
            ImGui::Begin("endlogo Window", &show_main_window, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMouseInputs);
            shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"endlogo");
            ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
            ImGui::Image(textureID, windowSize); // 텍스쳐의 크기를 조정할 수 있습니다.
            ImGui::End();
            ImGui::PopStyleColor();
        }
        //텍스트
        {
            //float windowSizeX = 800;
            //float windowSizeY = 150;
            //// Set the window size to a fixed value
            //ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
            //ImGui::SetNextWindowPos(ImVec2(100, 50), ImGuiCond_Always);
            //float r = 200.0f / 255.0f;
            //float g = 15.0f / 255.0f;
            //float b = 15.0f / 255.0f;
            //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.0f));
            //ImGui::Begin("TitleWindow", &show_main_window, ImGuiWindowFlags_NoDecoration);
            //
            //ImGui::SetWindowFontScale(5.0f);
            //{
            //    wchar_t buffer[256] = L"World of VVocano";
            //    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
            //    char* charBuffer = new char[bufferSize];
            //    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
            //    ImGui::TextColored(ImVec4(r, g, b, 1.0f), charBuffer);
            //    delete[] charBuffer;
            //}
            //ImGui::End();
            //ImGui::PopStyleColor();
        }
    }

    //show_all_window
    if (show_all_window)
    {
        // show_hp_window
        if (show_hp_window)
        {
            // Set the window size to a fixed value
            ImGui::SetNextWindowSize(ImVec2(310, 115), ImGuiCond_Always);
            // Set the window position to the top-left corner
            ImGui::SetNextWindowPos(ImVec2(-5, -5), ImGuiCond_Always);

            // 윈도우의 배경색을 투명도(alpha)에 따라 설정
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));


            wstring charImgPath = RESOURCES_ADDR_TEXTURE;
            charImgPath += L"CharSlot.PNG";
            shared_ptr<Texture> slotImg = MANAGER_RESOURCES()->LoadResource<Texture>(L"SkillSlot", charImgPath);

            wstring emptyBarImgPath = RESOURCES_ADDR_TEXTURE;
            emptyBarImgPath += L"HPMPEMPTYBAR.PNG";
            shared_ptr<Texture> EmtyBarImg = MANAGER_RESOURCES()->LoadResource<Texture>(L"EmptyBar", emptyBarImgPath);
            wstring HpBarImgPath = RESOURCES_ADDR_TEXTURE;
            HpBarImgPath += L"HPLINE.PNG";
            shared_ptr<Texture> HPBarImg = MANAGER_RESOURCES()->LoadResource<Texture>(L"HpBar", HpBarImgPath);
            wstring MpBarImgPath = RESOURCES_ADDR_TEXTURE;
            MpBarImgPath += L"MPLINE.PNG";
            shared_ptr<Texture> MPbarImg = MANAGER_RESOURCES()->LoadResource<Texture>(L"MpBar", MpBarImgPath);
            ImGui::Begin("Hp Window", &show_hp_window, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoScrollWithMouse| ImGuiWindowFlags_NoScrollbar);

            ImGui::SetCursorPos(ImVec2(-10, -10));
            ImGui::Image((void*)slotImg->GetTexture().Get(),ImVec2(310,115));
            ImGui::PopStyleColor(3); // Push한 스타일을 복원

            ImGui::SetCursorPos(ImVec2(15, 5));
            ImGui::Text("Warrior");
            //Hp Bar
            {
                ImVec2 curPos = ImGui::GetCursorPos();
                ImGui::SetCursorPosX(10);
                ImGui::Text("HP");
                curPos.x = 50;
                ImGui::SetCursorPos(curPos);
                ImGui::Image((void*)EmtyBarImg->GetTexture().Get(), ImVec2(240, 30));
                ImGui::SetCursorPos(curPos);
                ImGui::Image((void*)HPBarImg->GetTexture().Get(), ImVec2(240*_hp, 30));
            }
            //Mp Bar
            {
                ImVec2 curPos = ImGui::GetCursorPos();
                ImGui::SetCursorPosX(10);
                ImGui::Text("MP");
                curPos.x = 50;
                ImGui::SetCursorPos(curPos);
                ImGui::Image((void*)EmtyBarImg->GetTexture().Get(), ImVec2(240, 30));
                ImGui::SetCursorPos(curPos);
                ImGui::Image((void*)MPbarImg->GetTexture().Get(), ImVec2(240 * _mp, 30));
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
                    ImGui::SameLine((ImGui::GetWindowWidth() / 2 - buttonSize.x / 2));
                    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
                    wchar_t buffer[256] = L"예";
                    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                    char* charBuffer = new char[bufferSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                    if (ImGui::Button(charBuffer, buttonSize))
                    {
                        _rebirthQueue.push(true);

                        show_death_window = false;
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
            float windowSizeX = 64.0f + 17.0f;
            float windowSizeY = 64.0f + 17.0f;
            float ImageSizeX = 64.0f;
            float ImageSizeY = 64.0f;
            float windowPosX = displaySize.x / 2 - (windowSizeX * 4) / 2;
            float windowPosY = displaySize.y - windowSizeY;
            float spacing = windowSizeX;

            float r = 125.0f / 255.0f;
            float g = 98.0f / 255.0f;
            float b = 87.0f / 255.0f;

            wstring slotImgPath = RESOURCES_ADDR_TEXTURE;
            slotImgPath += L"SkillSlot.png";
            shared_ptr<Texture> slotImg = MANAGER_RESOURCES()->LoadResource<Texture>(L"SkillSlot", slotImgPath);


            wstring frameImgPath = RESOURCES_ADDR_TEXTURE;
            frameImgPath += L"Frame_red.PNG";
            shared_ptr<Texture> frameImg = MANAGER_RESOURCES()->LoadResource<Texture>(L"SkillFrame", frameImgPath);

            wstring coolDownImgPath = RESOURCES_ADDR_TEXTURE;
            coolDownImgPath += L"CoolDownBar.PNG";
            shared_ptr<Texture> cooldownImg = MANAGER_RESOURCES()->LoadResource<Texture>(L"CooldownBar", coolDownImgPath);


            ImGui::SetNextWindowSize(ImVec2(windowSizeX*6, windowSizeY+25), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(windowPosX-7.5, windowPosY-5), ImGuiCond_Always);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.0f));
            ImGui::Begin("SkillSlot", &show_skill_window, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse|ImGuiWindowFlags_NoMouseInputs);

            ImGui::SetCursorPos(ImVec2(3, 3));
            ImGui::Image((void*)slotImg->GetTexture().Get(), ImVec2((windowSizeX * 4)+10, windowSizeY+30));
            //Skill1
            {
                ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(windowPosX, windowPosY), ImGuiCond_Always);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.0f));
                ImGui::Begin("Skill1 Window", &show_skill_window, ImGuiWindowFlags_NoDecoration);
                shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"skill_charge");
                ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
                ImGui::SetCursorPos(ImVec2(3, 3));
                ImGui::Image(textureID, ImVec2(windowSizeX - 6, windowSizeY - 6));
                ImGui::SetCursorPos(ImVec2(3, 3));
                ImGui::Image((void*)frameImg->GetTexture().Get(), ImVec2(windowSizeX - 6, windowSizeY - 6));
                if (_playerAbilitySlot.lock()) {
                    auto& _ability = _playerAbilitySlot.lock()->GetAbility(0);
                    bool isCoolDown = _ability->IsCoolTime();
                    if (isCoolDown) {
                        float _abCooltime =_ability->GetCoolTime();
                        float _abCooltimer = _ability->GetCoolTimer();
                        ImGui::SetCursorPos(ImVec2(3, 3));
                        ImGui::Image((void*)cooldownImg->GetTexture().Get(), ImVec2(windowSizeX - 6, (windowSizeY - 6)*(1-(_abCooltimer/_abCooltime))));
                    }
                }
                ImGui::SetCursorPos(ImVec2(9, 3));
                ImGui::TextColored(ImVec4(1, 1, 1, 1), "1");
                ImGui::End();
                ImGui::PopStyleColor();
            }
            //Skill2
            {
                ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(windowPosX + spacing, windowPosY), ImGuiCond_Always);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.0f));
                ImGui::Begin("Skill2 Window", &show_skill_window, ImGuiWindowFlags_NoDecoration);
                shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"skill_shouting");
                ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
                ImGui::SetCursorPos(ImVec2(3, 3));
                ImGui::Image(textureID, ImVec2(windowSizeX - 6, windowSizeY - 6));
                ImGui::SetCursorPos(ImVec2(3, 3));
                ImGui::Image((void*)frameImg->GetTexture().Get(), ImVec2(windowSizeX - 6, windowSizeY - 6));
                if (_playerAbilitySlot.lock()) {
                    auto& _ability = _playerAbilitySlot.lock()->GetAbility(1);
                    bool isCoolDown = _ability->IsCoolTime();
                    if (isCoolDown) {
                        float _abCooltime = _ability->GetCoolTime();
                        float _abCooltimer = _ability->GetCoolTimer();
                        ImGui::SetCursorPos(ImVec2(3, 3));
                        ImGui::Image((void*)cooldownImg->GetTexture().Get(), ImVec2(windowSizeX - 6, (windowSizeY - 6) * (1 - (_abCooltimer / _abCooltime))));
                    }
                }
                ImGui::SetCursorPos(ImVec2(9, 3));
                ImGui::TextColored(ImVec4(1, 1, 1, 1), "2");
                ImGui::End();
                ImGui::PopStyleColor();
            }
            //Skill3
            {
                ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(windowPosX + spacing * 2, windowPosY), ImGuiCond_Always);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.0f));
                ImGui::Begin("Skill3 Window", &show_skill_window, ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoScrollWithMouse);
                shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"HealthPotion");
                ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
                ImGui::SetItemAllowOverlap();
                ImGui::SetCursorPos(ImVec2(3, 3));
                ImGui::Image(textureID, ImVec2(windowSizeX - 6, windowSizeY - 6));
                ImGui::SetCursorPos(ImVec2(3, 3));
                ImGui::Image((void*)frameImg->GetTexture().Get(), ImVec2(windowSizeX - 6, windowSizeY - 6));
                if (_playerAbilitySlot.lock()) {
                    auto& _ability = _playerAbilitySlot.lock()->GetAbility(2);
                    bool isCoolDown = _ability->IsCoolTime();
                    if (isCoolDown) {
                        float _abCooltime = _ability->GetCoolTime();
                        float _abCooltimer = _ability->GetCoolTimer();
                        ImGui::SetCursorPos(ImVec2(3, 3));
                        ImGui::Image((void*)cooldownImg->GetTexture().Get(), ImVec2(windowSizeX - 6, (windowSizeY - 6) * (1 - (_abCooltimer / _abCooltime))));
                    }
                }
                ImGui::SetCursorPos(ImVec2(9, 3));
                ImGui::TextColored(ImVec4(1, 1, 1, 1), "3");
                ImGui::End();
                ImGui::PopStyleColor();
            }
            //Skill4
            {
                ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(windowPosX + spacing * 3, windowPosY), ImGuiCond_Always);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.0f));
                ImGui::Begin("Skill4 Window", &show_skill_window, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
                shared_ptr<Texture> tex = MANAGER_RESOURCES()->GetResource<Texture>(L"ManaPotion");
                ImGui::SetItemAllowOverlap();
                ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->GetTexture().Get());
                ImGui::SetCursorPos(ImVec2(3, 3));
                ImGui::Image(textureID, ImVec2(windowSizeX - 6, windowSizeY - 6));
                ImGui::SetCursorPos(ImVec2(3, 3));
                ImGui::Image((void*)frameImg->GetTexture().Get(), ImVec2(windowSizeX-6 , windowSizeY-6));
                if (_playerAbilitySlot.lock()) {
                    auto& _ability = _playerAbilitySlot.lock()->GetAbility(3);
                    bool isCoolDown = _ability->IsCoolTime();
                    if (isCoolDown) {
                        float _abCooltime = _ability->GetCoolTime();
                        float _abCooltimer = _ability->GetCoolTimer();
                        ImGui::SetCursorPos(ImVec2(3, 3));
                        ImGui::Image((void*)cooldownImg->GetTexture().Get(), ImVec2(windowSizeX - 6, (windowSizeY - 6) * (1 - (_abCooltimer / _abCooltime))));
                    }
                }
                ImGui::SetCursorPos(ImVec2(9, 3));
                ImGui::TextColored(ImVec4(1, 1, 1, 1), "4");
                ImGui::End();
                ImGui::PopStyleColor();
            }
            ImGui::End();
            ImGui::PopStyleColor();
        }

        //show_quest_window
        if (show_quest_window)
        {
            ImVec2 displaySize = ImGui::GetIO().DisplaySize;

            {
                {
                    float windowSizeX = 220.0f;
                    float windowSizeY = 220.0f;
                    float windowPosX = displaySize.x - (windowSizeX + 10.0f);
                    float windowPosY = 50.0f;

                    float r = 50.0f / 255.0f;
                    float g = 50.0f / 255.0f;
                    float b = 50.0f / 255.0f;

                    // Set the window size to a fixed value
                    ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
                    ImGui::SetNextWindowPos(ImVec2(windowPosX, windowPosY), ImGuiCond_Always);
                    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.5f));
                    ImGui::Begin("Quest Window", &show_quest_window, ImGuiWindowFlags_NoDecoration);
                }

                ImGui::SetWindowFontScale(1.5f);
                {
                    wchar_t buffer[256] = L" Quest";
                    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                    char* charBuffer = new char[bufferSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                    float r = 255.0f / 255.0f;
                    float g = 210.0f / 255.0f;
                    float b = 1.0f / 255.0f;
                    ImGui::TextColored(ImVec4(r, g, b, 1.0f), charBuffer);
                    delete[] charBuffer;
                }

                ImGui::SetWindowFontScale(1.0f);
                {
                    wchar_t buffer[256] = L"라그나로스 소환";
                    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                    char* charBuffer = new char[bufferSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                    float r = 255.0f / 255.0f;
                    float g = 210.0f / 255.0f;
                    float b = 1.0f / 255.0f;
                    ImGui::TextColored(ImVec4(r, g, b, 1.0f), charBuffer);
                    delete[] charBuffer;
                }

                ImGui::SetWindowFontScale(1.0f);
                {
                    wchar_t buffer[256];
                    swprintf(buffer, 256, L"남작 게돈을 쓰러뜨려라!\n남작 게돈(%d/1)", geddonCount);
                    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                    char* charBuffer = new char[bufferSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                    float r = 255.0f / 255.0f;
                    float g = 255.0f / 255.0f;
                    float b = 255.0f / 255.0f;
                    ImGui::TextColored(ImVec4(r, g, b, 1.0f), charBuffer);
                    delete[] charBuffer;
                }
                ImGui::End();
                ImGui::PopStyleColor();
            }
        }

        if (show_dialogue_window) {
            ImVec2 displaySize = ImGui::GetIO().DisplaySize;
            float windowSizeX = 500.0f;
            float windowSizeY = 200.0f;
            float windowPosX = displaySize.x / 2 - windowSizeX / 2;
            float windowPosY = displaySize.y / 2 - windowSizeY / 2;
            float r = 143.0f / 255.0f;
            float g = 114.0f / 255.0f;
            float b = 88.0f / 255.0f;

            //이름표
            {
                ImGui::SetNextWindowSize(ImVec2(80, 50), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(windowPosX, windowPosY - 60), ImGuiCond_Always);
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.8f));
                ImGui::Begin("Name Window", &show_dialogue_window, ImGuiWindowFlags_NoDecoration);
                ImGui::SetWindowFontScale(1.25f);
                wchar_t buffer[256] = L"마그니";
                int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                char* charBuffer = new char[bufferSize];
                WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                float r = 255.0f / 255.0f;
                float g = 255.0f / 255.0f;
                float b = 255.0f / 255.0f;
                ImGui::TextColored(ImVec4(r, g, b, 1.0f), charBuffer);
                delete[] charBuffer;
                ImGui::End();
                ImGui::PopStyleColor();
            }

            ImGui::SetNextWindowSize(ImVec2(windowSizeX, windowSizeY), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(windowPosX, windowPosY), ImGuiCond_Always);
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(r, g, b, 0.8f));
            ImGui::Begin("Dialogue Window", &show_dialogue_window, ImGuiWindowFlags_NoDecoration);

            // 현재 출력 중인 스크립트
            std::wstring& script = scripts[currentScriptIndex];

            // 시간당 출력되는 글자 수를 고려하여 현재 시각에서 출력되어야 할 글자 수 계산
            if (currentCharTime >= printTime)
            {
                if (currentCharIndex < script.size())
                {
                    ++currentCharIndex;
                }
                currentCharTime = 0.0f;
            }
            else
            {
                currentCharTime += MANAGER_TIME()->GetDeltaTime();
            }

            // 스크립트를 점진적으로 출력
            std::wstring buffer;
            for (int i = 0; i < currentCharIndex && i < script.size(); ++i) {
                buffer += script[i];
            }

            // ImGui로 출력
            ImGui::SetWindowFontScale(1.25f);
            wchar_t charBuffer[256];
            wcscpy_s(charBuffer, buffer.c_str());
            int bufferSize = WideCharToMultiByte(CP_UTF8, 0, charBuffer, -1, nullptr, 0, nullptr, nullptr);
            char* outBuffer = new char[bufferSize];
            WideCharToMultiByte(CP_UTF8, 0, charBuffer, -1, outBuffer, bufferSize, nullptr, nullptr);
            r = 255.0f / 255.0f;
            g = 255.0f / 255.0f;
            b = 255.0f / 255.0f;
            ImGui::TextColored(ImVec4(r, g, b, 1.0f), "%s", outBuffer);

            if (currentScriptIndex >= scripts.size() - 1 && currentCharIndex >= script.size()) {
                ImVec2 buttonSize(100.0f, 30.0f);
                float r = 208 / 255.0f;
                float g = 171 / 255.0f;
                float b = 156 / 255.0f;
                ImVec4 buttonColor(r, g, b, 0.5f);
                ImGui::NewLine();
                ImGui::NewLine();
                ImGui::SameLine((ImGui::GetWindowWidth() / 2 - buttonSize.x));
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
                    wchar_t buffer[256] = L"예";
                    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, nullptr, 0, nullptr, nullptr);
                    char* charBuffer = new char[bufferSize];
                    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, charBuffer, bufferSize, nullptr, nullptr);
                    if (ImGui::Button(charBuffer, buttonSize))
                    {
                        show_dialogue_window = false;
                        show_quest_window = true;
                        currentScriptIndex = 0;
                        currentCharIndex = 0;
                        currentCharTime = 0;
                        _changeSceneQueue.push(true);
                        
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
                        show_dialogue_window = false;
                        currentScriptIndex = 0; // 스크립트 인덱스
                        currentCharIndex = 0; // 글자 인덱스
                        currentCharTime = 0;
                    }
                    delete[] charBuffer;
                    ImGui::PopStyleColor();
                }
            }

            ImGui::End();
            ImGui::PopStyleColor();

            if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::ENTER))
            {
                if (currentCharIndex < script.size())
                {
                    currentCharIndex = script.size();
                    
                }
                else if (currentScriptIndex < scripts.size() - 1)
                {
                    currentCharIndex = 0;
                    ++currentScriptIndex;
                }
                currentCharTime = 0.0f;
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

void ImGuiManager::UpdatePicked(bool isPicked, const shared_ptr<GameObject>& pickObj)
{
    if (pickObj->GetObjectType() == ObjectType::PlayableUnit)
    {
        auto& infomation = pickObj->GetComponent<CharacterInfo>()->GetCharacterInfo();
        _hp = static_cast<float>(infomation._hp) / static_cast<float>(infomation._maxHp);
        _mp = static_cast<float>(infomation._mp) / static_cast<float>(infomation._maxMp);

    }
    else if(pickObj->GetObjectType() == ObjectType::EnemyUnit)
    {
        auto infomation = pickObj->GetComponent<CharacterInfo>()->GetCharacterInfo();
        show_picked_hp_window = isPicked;
        _pickedHp = static_cast<float>(infomation._hp) / static_cast<float>(infomation._maxHp);

        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, infomation._name.c_str(), -1, nullptr, 0, nullptr, nullptr);
        _name = new char[bufferSize];
        WideCharToMultiByte(CP_UTF8, 0, infomation._name.c_str(), -1, _name, bufferSize, nullptr, nullptr);
    }

}

int ImGuiManager::GetRebirthQueueSize()
{
    if (_rebirthQueue.empty() == false)
    {
        int queueSize = _rebirthQueue.size();

        while (_rebirthQueue.empty() == false)
        {
            _rebirthQueue.pop();
        }

        return queueSize;
    }

    return -1;
}

int ImGuiManager::GetChangeSceneQueueSize()
{
    if (_changeSceneQueue.empty() == false)
    {
        int queueSize = _changeSceneQueue.size();
        _changeSceneQueue.pop();
        return queueSize;
    }

    return -1;
}
