//
// Created by guiwen on 11/24/20.
//

#include <iostream>
#include "json/json.h"
#include <fstream>
std::string MPT_test_createJson(void)
{
    std::string jsonStr;
    Json::Value root, fruit, mail;
    Json::StreamWriterBuilder writerBuilder;
    std::ostringstream os;
    //自动换行
    writerBuilder.settings_["indentation"] = "\t";

    root["Name"] = "Liangbaikai";
    root["Age"] = 25;
    root["gender"] = "man";

    fruit[0] = "apple";
    fruit[1] = "orange";
    fruit[2] = "banana";
    root["fruit"] = fruit;

    mail["QQ"] = "XXX@qq.com";
    mail["Google"] = "XXX@gmail.com";
    root["mail"] = mail;

    // jsonWriter是智能指针, 当jsonWriter被析构时, 它所指向的对象(内存)也被自动释放
    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os);
    jsonStr = os.str();

    std::cout << "Json:" << jsonStr << std::endl;
    return jsonStr;
}
void MPT_test_saveJson(std::string file, std::string& jsonStr)
{
    std::ofstream ofs;
    ofs.open(file);
    if (!ofs.is_open())
    {
        std::cout << "open json file failed." << std::endl;
        return;
    }
    ofs << jsonStr;
    ofs.close();
    return;
}
void MPT_test_readJson(std::string file) {
    std::ifstream ifs;
    ifs.open(file, std::ios::in);
    if (!ifs.is_open())
    {
        std::cout << "open json file failed." << std::endl;
        return;
    }

    Json::CharReaderBuilder reader;
    Json::Value root, fruit, mail;

    int iage = 0;
    std::string sname;
    JSONCPP_STRING errs;

    bool res = Json::parseFromStream(reader, ifs, &root, &errs);
    if (!res || !errs.empty())
    {
        std::cout << "parseJson error! " << errs << std::endl;
        return;
    }

    iage = root["Age"].asInt();
    sname = root["Name"].asString();
    fruit = root["fruit"];
    mail = root["mail"];
    ifs.close();

    std::cout << "age: " << iage << std::endl;
    std::cout << "name: " << sname << std::endl;
    std::cout << "fruit: " << fruit[0] << fruit[1] << fruit[2] << std::endl;
    std::cout << "mailqq:" << mail["QQ"] << std::endl;
    return;
}
void MPT_test_changeJson(std::string file) {
    std::ifstream ifs;
    ifs.open(file, std::ios::in);
    if (!ifs.is_open())
    {
        std::cout << "open json file failed." << std::endl;
        return;
    }

    Json::CharReaderBuilder reader;
    Json::Value root;

    JSONCPP_STRING errs;

    bool res = Json::parseFromStream(reader, ifs, &root, &errs);
    if (!res || !errs.empty())
    {
        std::cout << "parseJson error! " << errs << std::endl;
        return;
    }

    root["Age"] = 30;
    root["Name"] = "Liangbaikai's liangbaikai";
    root["fruit"][0] = "strawberry";
    ifs.close();

    std::ofstream os;
    os.open(file, std::ios::out);
    if (!os.is_open())
    {
        std::cout << "open json file failed." << std::endl;
        return;
    }
    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(root, &os);
    return;
}
void readStrProJson()
{
    std::string strValue = "{\"name\":\"shuiyixin\",\"major\":[{\"AI\":\"MachineLearning\"},{\"AI\":\"DeepLearning\"},{\"AI\":\"ComputerVision\"}]}";
//    Json::Reader reader;
//    Json::Value value;
//    if (reader.parse(strValue, value))
//    {
//        std::string out = value["name"].asString();
//        std::cout << out << std::endl;
//        const Json::Value arrayObj = value["major"];
//        for (unsigned int i = 0; i < arrayObj.size(); i++)
//        {
//            out = arrayObj[i]["AI"].asString();
//            std::cout << out<<std::endl;
//        }
//    }

//    const std::string rawJson = R"({"Age": 20, "Name": "colin"})";
    JSONCPP_STRING err;
    Json::Value root;

    Json::Value value;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (reader->parse(strValue.c_str(), strValue.c_str() + strValue.length(), &value,
                       &err)) {

        std::string out = value["name"].asString();
        std::cout << out << std::endl;
        const Json::Value arrayObj = value["major"];
        for (unsigned int i = 0; i < arrayObj.size(); i++)
        {
            out = arrayObj[i]["AI"].asString();
            std::cout << out<<std::endl;
        }
    }
    Json::Reader reader1;
    reader1.parse(strValue, root);
    std::string out = value["name"].asString();
    std::cout << out << std::endl;
    const Json::Value arrayObj = value["major"];
    for (unsigned int i = 0; i < arrayObj.size(); i++)
    {
        out = arrayObj[i]["AI"].asString();
        std::cout << out<<std::endl;
    }
    Json::StreamWriterBuilder builder1;
    const std::string json_file = Json::writeString(builder1, root);
    std::cout << json_file << std::endl;


//    const char * jsongroupinfo="[{/"groupId/" :946838524,/"groupname/" :/"bababa/", /"mask/":1,/"parentid/":946755072}]";

    Json::Reader reader2;
    Json::Value json_object;
//    if (!reader2.parse(jsongroupinfo, json_object))
//        return ;
//    SUserChggroup sucg;
//    VECTOR< SUserChggroup > m_groupInfo;
//    for(int i = 0; i < json_object.size(); i ++)
//    {
//        Json::Value &current = json_object[i];
//        sucg.m_groupId = current["groupId"].asInt();
//        sucg.m_groupName = current["groupname"].asString();
//        sucg.m_mask = current["mask"].asInt();
//        sucg.m_parentId = current["parentid"].asInt();
//        m_groupInfo.push_back(sucg);
//    }
//    const std::string name = root["Name"].asString();
//    const int age = root["Age"].asInt();
//
//    std::cout << name << std::endl;
//    std::cout << age << std::endl;
}
int main()
{
    readStrProJson();
//    std::string jsonStr = MPT_test_createJson();
//    MPT_test_saveJson("../json/json-none.json", jsonStr);
//    MPT_test_readJson("../json/json-none.json");
//    MPT_test_changeJson("../json/json-none.json");
    return 0;
}