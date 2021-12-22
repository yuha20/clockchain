//
// Created by Dave Nash on 20/10/2017.
//

#include "Block.h"
#include "sha256.h"

Block::Block(uint32_t nIndexIn, const string &sDataIn) : _nIndex(nIndexIn), _sData(sDataIn) {
    _nNonce = 0;
    _tTime = time(nullptr);

    sHash = _CalculateHash();
}
// construct the block by a JSON string
Block::Block(string jsonString) {
    JSONCPP_STRING err;
    Json::Value value;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (reader->parse(jsonString.c_str(), jsonString.c_str() + jsonString.length(), &value,
                      &err)) {
        sHash = value["sHash"].asString();
        sPrevHash = value["sPrevHash"].asString();
        _tTime = value["_tTime"].asInt64();
        _nNonce = value["_nNonce"].asInt();
        _nIndex = value["_nIndex"].asInt();
        _sData = value["_sData"].asString();
    }
}
// construct the block by JSON object
Block::Block(Json::Value &value) {
    sHash = value["sHash"].asString();
    sPrevHash = value["sPrevHash"].asString();
    _tTime = value["_tTime"].asInt64();
    _nNonce = value["_nNonce"].asInt();
    _nIndex = value["_nIndex"].asInt();
    _sData = value["_sData"].asString();
}
//return a JSON string of the block
string Block::GetJsonString() {
    std::string jsonStr;
    Json::Value root, chain;
    Json::StreamWriterBuilder writerBuilder;
    std::ostringstream os;
    //auto rewind
    writerBuilder.settings_["indentation"] = "\t";

    chain["sHash"] = sHash;
    chain["sPrevHash"] = sPrevHash;
    chain["_tTime"] = _tTime;
    chain["_nNonce"] = _nNonce;
    chain["_nIndex"] = _nIndex;
    chain["_sData"] = _sData;

    // smart pointer, will be released automatically
    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(chain, &os);
    jsonStr = os.str();

    std::cout << "Json:" << jsonStr << std::endl;
    return jsonStr;
}
//return a JSON object of the block
Json::Value Block::toJsonObj() {
    Json::Value chain;
    chain["sHash"] = sHash;
    chain["sPrevHash"] = sPrevHash;
    chain["_tTime"] = _tTime;
    chain["_nNonce"] = _nNonce;
    chain["_nIndex"] = _nIndex;
    chain["_sData"] = _sData;
    return chain;
}
bool Block::IsValidate() {
    return sHash == _CalculateHash();
}

void Block::MineBlock(uint32_t nDifficulty) {
    char cstr[nDifficulty + 1];
    for (uint32_t i = 0; i < nDifficulty; ++i) {
        cstr[i] = '0';
    }
    cstr[nDifficulty] = '\0';

    string str(cstr);
    cout << "Mining... " << endl;
    do {
        _nNonce++;
        sHash = _CalculateHash();
    } while (sHash.substr(0, nDifficulty) != str);

    cout << "Block mined: " << sHash << endl;
}

inline string Block::_CalculateHash() const {
    stringstream ss;
    ss << _nIndex << sPrevHash << _tTime << _sData << _nNonce;

    return sha256(ss.str());
}
