//
// Created by Dave Nash on 20/10/2017.
//

#ifndef TESTCHAIN_BLOCKCHAIN_H
#define TESTCHAIN_BLOCKCHAIN_H

#include <cstdint>
#include <vector>
#include <iostream>
#include <mutex>
#include <string>
#include <condition_variable>
#include <queue>
#include "json/json.h"
#include <fstream>
#include "Block.h"
using namespace std;
const int BUFFER_SIZE = 100;
const int BLOCK_SIZE = 1000;
const int TIMER_INTERVAL =1; //second
class Blockchain {
public:
    Blockchain();
    void AddBlock(Block bNew);
    //recover from file
    void Load(std::string fileName);
    void Save(std::string fileName);
    void SaveOneBlock(std::string fileName,Block block);
    bool IsValidate();
    bool isAutorized(string token);
    string  ReceivedRequest(string);
    void TimerTask();
    void MiningTask();

private:
    mutex _mutex;
    queue<string> _requestQueue;
    uint32_t _nIndex;
    uint32_t _nDifficulty;
    vector<Block> _vChain;

    Block _GetLastBlock() const;
    std::string GetJsonString(void);
};

#endif //TESTCHAIN_BLOCKCHAIN_H
