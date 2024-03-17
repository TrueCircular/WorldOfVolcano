#include "pch.h"
#include "CharacterInfo.h"
#include <filesystem>
#include "Utils.h"

CharacterInfo::CharacterInfo()
{

}

CharacterInfo::~CharacterInfo()
{

}

void CharacterInfo::InitInformation()
{
}

bool CharacterInfo::LoadCharacterInformationFromFile(const wstring& loadPath)
{
    //Set LoadPath
    wstring LoadPath = wstring(loadPath);
    auto tempPath = filesystem::path(LoadPath);

    //Create Document
    shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

    //ErrorCheck & LoadFile
    ::XMLError errorCheck = document->LoadFile(tempPath.string().c_str());
    if (errorCheck != ::XML_SUCCESS)
    {
        return false;
    }
	else
	{
		::XMLElement* root = document->FirstChildElement();
		::XMLElement* InfoNode = root->FirstChildElement();
		{
			CHARACTER_INFO tempInfo;

			//Find FirstNode
			::XMLNode* node = document->FirstChild();
			//Find Element
			//Name
			::XMLElement* element = node->FirstChildElement("Name");
			string temp;
			temp = element->GetText();
			//tempInfo._name = Utils::ToWString(temp);

			//HP
			element = node->FirstChildElement("HP");
			unsigned int hp = 0;
			element->QueryUnsignedText(&hp);
			tempInfo._hp = static_cast<uint32>(hp);
			tempInfo._maxHp = tempInfo._hp;

			//MP
			element = node->FirstChildElement("MP");
			unsigned int mp = 0;
			element->QueryUnsignedText(&mp);
			tempInfo._mp = static_cast<uint32>(mp);
			tempInfo._maxMp = tempInfo._mp;

			//ATK
			element = node->FirstChildElement("ATK");
			int atk = 0;
			element->QueryIntText(&atk);
			tempInfo._atk = static_cast<uint16>(atk);

			//DEF
			element = node->FirstChildElement("DEF");
			int def = 0;
			element->QueryIntText(&def);
			tempInfo._def = static_cast<uint16>(def);

			//MoveSpeed
			element = node->FirstChildElement("MoveSpeed");
			int moveSpeed = 0;
			element->QueryIntText(&moveSpeed);
			tempInfo._moveSpeed = static_cast<uint16>(moveSpeed);

			//AggroPow
			element = node->FirstChildElement("AggroPow");
			int aggroPow = 0;
			element->QueryIntText(&aggroPow);
			tempInfo._aggroLevel = static_cast<uint16>(aggroPow);

			//AttackRange
			element = node->FirstChildElement("AttackRange");
			float attackRange = 0;
			element->QueryFloatText(&attackRange);
			tempInfo._attackRange = attackRange;

			//AttackTime
			element = node->FirstChildElement("AttackTime");
			float attackTime = 0;
			element->QueryFloatText(&attackTime);
			//tempInfo._attackTime = attackTime;


			//_info._name = tempInfo._name;
			_info._hp = tempInfo._hp;
			_info._maxHp = tempInfo._maxHp;
			_info._atk = tempInfo._atk;
			_info._def = tempInfo._def;
			_info._moveSpeed = tempInfo._moveSpeed;
			_info._aggroLevel = tempInfo._aggroLevel;
			_info._attackRange = tempInfo._attackRange;
			//_info._attackTime = tempInfo._attackTime;
		}
	}

	return true;
}

bool CharacterInfo::SaveCharacterInformationFromFile(const wstring& savePath)
{
	//Set SavePath
	wstring SavePath = wstring(savePath);
	auto tempPath = filesystem::path(SavePath);

	//Create Document
	shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();
    //Create Save Data 
    {
        ::XMLNode* node = document->NewElement("CharacterInfo");
        document->InsertFirstChild(node);

		::XMLElement* element = document->NewElement("Name");
		//element->SetText(Utils::ToString(_info._name).c_str());
		element->SetText(Utils::ToString(L"test").c_str());
		node->InsertEndChild(element);

		element = document->NewElement("HP");
		element->SetText(_info._hp);
		node->InsertEndChild(element);

		element = document->NewElement("MP");
		element->SetText(_info._mp);
		node->InsertEndChild(element);

		element = document->NewElement("ATK");
		element->SetText(_info._atk);
		node->InsertEndChild(element);

		element = document->NewElement("DEF");
		element->SetText(_info._def);
		node->InsertEndChild(element);

		element = document->NewElement("MoveSpeed");
		element->SetText(_info._moveSpeed);
		node->InsertEndChild(element);

		element = document->NewElement("AggroPow");
		element->SetText(_info._aggroLevel);
		node->InsertEndChild(element);

		element = document->NewElement("AttackRange");
		element->SetText(_info._attackRange);
		node->InsertEndChild(element);

		/*element = document->NewElement("AttackTime");
		element->SetText(_info._attackTime);
		node->InsertEndChild(element);*/
    }

	//Save Data
	::XMLError errorCheck = document->SaveFile(tempPath.string().c_str());

	//ErrorCheck
	if (errorCheck != ::XML_SUCCESS)
	{
		return false;
	}

	return true;
}
