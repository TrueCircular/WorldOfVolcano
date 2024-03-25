#include "pch.h"
#include "AbilityData.h"
#include <filesystem>
#include "Utils.h"

AbilityData::AbilityData()
{
}

AbilityData::~AbilityData()
{
}

bool AbilityData::LoadAbilityDataFromFile(const wstring& path)
{
    //Set LoadPath
    wstring LoadPath = wstring(path);
    auto tempPath = filesystem::path(path);

    //Create Document
    shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

    //ErrorCheck & LoadFile
    ::XMLError errorCheck = document->LoadFile(tempPath.string().c_str());
    if (errorCheck != ::XML_SUCCESS)
    {
        return false;
    }

    //Load Ability Data
    {
        AbilityDataDesc tempDesc;

        //Find FirstNode
        ::XMLNode* node = document->FirstChild();

        //Find Element//
        //Name
        ::XMLElement* element = node->FirstChildElement("Name");
        string temp;
        temp = element->GetText();
        tempDesc.Name = Utils::ToWString(temp);

        //AbilityType
        element = node->FirstChildElement("AbilityType");
        int iType = 0;
        element->QueryIntText(&iType);
        tempDesc.Type = static_cast<AbilityType>(iType);

        //Consumed Mp
        element = node->FirstChildElement("ConsumedMp");
        unsigned int mp = 0;
        element->QueryUnsignedText(&mp);
        tempDesc.ConsumedMp = static_cast<uint32>(mp);

        //Ability CoolTime
        element = node->FirstChildElement("AbilityCoolTime");
        float coolTime = 0;
        element->QueryFloatText(&coolTime);
        tempDesc.AbilityCoolTime = coolTime;

        //Ability Pow
        element = node->FirstChildElement("AbilityPow");
        float pow = 0;
        element->QueryFloatText(&pow);
        tempDesc.AbilityPow = pow;

        //Ability Range
        element = node->FirstChildElement("AbilityRange");
        float range = 0;
        element->QueryFloatText(&range);
        tempDesc.AbilityRange = range;

        _desc.Name = tempDesc.Name;
        _desc.Type = tempDesc.Type;
        _desc.ConsumedMp = tempDesc.ConsumedMp;
        _desc.AbilityCoolTime = tempDesc.AbilityCoolTime;
        _desc.AbilityPow = tempDesc.AbilityPow;
        _desc.AbilityRange = tempDesc.AbilityRange;
    }

    return true;
}

bool AbilityData::SaveAbilityDataToFilePath(const wstring& path)
{
    //Set SavePath
    wstring SavePath = wstring(path);
    auto tempPath = filesystem::path(path);
    auto directoryPath = tempPath.parent_path();

    if (std::filesystem::exists(directoryPath) == false)
    {
        if (std::filesystem::create_directories(directoryPath) == false)
        {
            return false;
        }
    }

    //Create Document
    shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

    //Create Save Data
    {
        ::XMLNode* node = document->NewElement("AbilityInformation");
        document->InsertFirstChild(node);

        ::XMLElement* element = document->NewElement("Name");
        element->SetText(Utils::ToString(_desc.Name).c_str());
        node->InsertEndChild(element);

        element = document->NewElement("AbilityType");
        element->SetText(static_cast<uint8>(_desc.Type));
        node->InsertEndChild(element);

        element = document->NewElement("ConsumedMp");
        element->SetText(static_cast<uint32>(_desc.ConsumedMp));
        node->InsertEndChild(element);

        element = document->NewElement("AbilityCoolTime");
        element->SetText((_desc.AbilityCoolTime));
        node->InsertEndChild(element);

        element = document->NewElement("AbilityPow");
        element->SetText((_desc.AbilityPow));
        node->InsertEndChild(element);

        element = document->NewElement("AbilityRange");
        element->SetText((_desc.AbilityRange));
        node->InsertEndChild(element);
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
