#pragma once
#include "GUIInterface.h"

class CharacterInfo;

class ImGuiManager
{
	using GuiList = vector<shared_ptr<GUIInterface>>;
private:
	static ImGuiManager* _instance;
public:
	static ImGuiManager* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new ImGuiManager();

		return _instance;
	}
private:
	ImGuiManager();
	~ImGuiManager();
public:
	void Init();
	void Update();
	void Render();

	void AddMessage(char message[50]);

	std::vector<std::string> GetLatestMessages() { return chatMessages; }
	void UpdateHp(uint32 maxHp, uint32 hp);
	void UpdateMp(uint32 maxMp, uint32 mp);
	void UpdatePicked(bool isPicked, uint32 maxHp = 0, uint32 hp = 0, wstring name = L"");
	void UpdatePicked(bool isPicked, const shared_ptr<GameObject>& pickObj);

	void NotifyPlayerAlive(bool isAlive) { show_death_window = !isAlive;}
	int GetAttackQueueSize();
	int GetChangeSceneQueueSize();
	bool CheckGameStart() { return _isGameStart; }
	void BeginDialogue() { show_dialogue_window = true; }
private:
	//Player Stat
	float _hp = 1.0f;
	float _mp = 1.0f;
	char* _name;
	//Picked Object Stat
	float _pickedHp = 1.0f;
	//Rebirth
	queue<bool> _rebirthQueue;
	queue<bool> _changeSceneQueue;
	//���� Ÿ��Ʋ
	bool _isGameStart = false;
	shared_ptr<Texture> _mainTexture;
	//��ü ������ Ȱ��ȭ ����
	bool show_all_window = false;
	bool show_main_window = true;
	//������ Ȱ��ȭ ����
	bool show_hp_window = true;
	bool show_picked_hp_window = false;
	bool show_chat_window = true;
	bool show_skill_window = true;
	bool show_quest_window = false;
	bool show_death_window = false;
	bool show_dialogue_window = false;
	bool show_nickname_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	//����Ʈ
	int geddonCount = 0;
	std::vector<std::wstring> scripts;
	int currentScriptIndex = 0; // ��ũ��Ʈ �ε���
	int currentCharIndex = 0; // ���� �ε���
	float currentCharTime = 0;
	float printTime = 0.05f;
	//ä�� ���
	char buffer[256] = "";
	char nameBuffer[256] = "";
	std::vector<std::string> chatMessages; // ä�� �޽����� ������ ����
	std::vector<std::string> otherChatMessages; // �ٸ� ���� ä�� �޽����� ������ ����
	bool isScrollBottom = false;
};