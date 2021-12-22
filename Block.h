//
// Created by Dave Nash on 20/10/2017.
//

#ifndef TESTCHAIN_BLOCK_H
#define TESTCHAIN_BLOCK_H

#include <cstdint>
#include <iostream>
#include <sstream>
#include "json/json.h"
using namespace std;

class Block {
public:
    string sHash;
    string sPrevHash;

    Block(uint32_t nIndexIn, const string &sDataIn);
    Block(Json::Value &value);
    Block(string jsonString);
    string GetJsonString();
    Json::Value toJsonObj();
    bool IsValidate();
    void MineBlock(uint32_t nDifficulty);

private:
    uint32_t _nIndex;
    uint32_t _nNonce;
    string _sData;
    time_t _tTime;

    string _CalculateHash() const;
};

#endif //TESTCHAIN_BLOCK_H
