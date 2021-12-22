//
// Created by Dave Nash on 20/10/2017.
//

#include "Blockchain.h"
#include <ctime>
#include <iomanip> // put_time
#include <thread>
Blockchain::Blockchain()
{
    _vChain.emplace_back(Block(0, "Genesis Block"));
    _nDifficulty = 6;
    _nIndex=1;
    Load("../json/blockchain.json");
    TimerTask();
    MiningTask();

}
// put a timer block task into the block request  queue in every minute
void Blockchain::TimerTask() {

    std::thread t([&]() {
        while(true) {
            unique_lock<mutex> lk(_mutex);
            if (_requestQueue.size() < BUFFER_SIZE)
            {
                auto t = chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                cout << "Timer block: " << ctime(&t) << endl;
                string content=ctime(&t);
                _requestQueue.push("CLIENTID:timerblock CONTENT:" + content);
            }
            else
            {
                cout << "### queue for block chain request is full, waiting " << endl;
            }
            lk.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000*60));
        }
    });
    t.detach();

}
//read a  block request from  queue and do the actual block mining work.
void Blockchain::MiningTask() {
    std::thread t([&]() {
        while (true)
        {
            string blockContent="";
            unique_lock<mutex> lk(_mutex);
            if (!_requestQueue.empty())
            {
                while (!_requestQueue.empty() && blockContent.size() < BLOCK_SIZE) {
                    //merge the request into blockContent according to the discussion in Pizza
                    blockContent += _requestQueue.front() + "$"; //$ is a separator for the minging
                    _requestQueue.pop();
                }
                lk.unlock();
            }
            else
            {
                lk.unlock();
                blockContent="";
                cout << "Miner is waiting for available task " << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000*TIMER_INTERVAL));
            }

            if (blockContent != "")
            {
                //do the real block mining task, and attach it  to the block chain.
                AddBlock( Block(_nIndex++, blockContent));
            }
        }
    });
    t.detach();

}
void Blockchain::AddBlock(Block bNew)
{
    bNew.sPrevHash = _GetLastBlock().sHash;
    bNew.MineBlock(_nDifficulty);
    _vChain.push_back(bNew);
    Save("../json/blockchain.json");

}

Block Blockchain::_GetLastBlock() const
{
    return _vChain.back();
}
// This method will be called when there is request from client
string Blockchain::ReceivedRequest(string request) {
    std::cout << request << std::endl;
    JSONCPP_STRING err;
    Json::Value value;
    Json::CharReaderBuilder builder;
    string response="";
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (reader->parse(request.c_str(), request.c_str() + request.length(), &value,
                      &err)) { //parsing the request according to a JSON protocol
        //the operation mode : :inquiry,content}
        string operation = value["op"].asString();
        //the client identification
        string clientid = value["clientid"].asString();
        //token used for client authorization
        string token = value["token"].asString();
        //the content from client,encrypted.
        string content = value["content"].asString();
        //the time stamp from client
        time_t t= value["time"].asInt64();
        if (isAutorized(token)) {
            if (operation == "inqury") { //inquery the block chain, the API to readd all the block chain in JSON
                response = GetJsonString();
            } else if (operation == "mining") //mining the content
            {
                unique_lock<mutex> lk(_mutex);
                if (_requestQueue.size() < BUFFER_SIZE) {
                    //push the mining request in the block request chain which will be processed in MiningTask
                    _requestQueue.push("CLIENTID:" + clientid + " CONTENT:" + content);
                    response = "ming request is received!";
                } else {
                    response = "### queue for block chain request is full, please send the request later! ";
                }
                lk.unlock();
            } else {
                response = "Invalid request! ";
            }
        } else{
            response = "Not autorized! ";
        }
    }

    return response;
}
bool Blockchain::isAutorized(string token)
{
    //here we assume all the tocken is autorized
    return true;
}
//check whehter the block chain valid by checking block hash
// and checking if the previous field "sPrevHash" of current block is equal to the field "sHash" of  previous block
bool  Blockchain::IsValidate()
{
    string sPrevHash="";
    for (unsigned int i = 0; i < _vChain.size(); i++)
    {
        // "sHash" field in block should be equal to  hashvalue of the  content in this block  otherwise it is invalid
        if (!_vChain[i].IsValidate())
            return false;
        //the previous field "sPrevHash" of current block should be equal to the field "sHash" of  previous block
        //otherwise, it is invalid
        if (sPrevHash!=_vChain[i].sPrevHash)
            return false;
        sPrevHash=_vChain[i].sHash;

    }
    return true;
}
//load the block chain from file in JSON format
void  Blockchain::Load(std::string file) {
    std::ifstream ifs;
    ifs.open(file, std::ios::in);
    if (!ifs.is_open())
    {
        std::cout << "open stored blockchain file failed." << std::endl;
        return;
    }

    Json::CharReaderBuilder reader;
    Json::Value root;

    JSONCPP_STRING errs;

    bool res = Json::parseFromStream(reader, ifs, &root, &errs);
    if (!res || !errs.empty())
    {
        std::cout << "parse blockchain file error! " << errs << std::endl;
        return;
    }
    _vChain.clear();
    for (unsigned int i = 0; i < root.size(); i++)
    {
        Block block=Block(root[i]);
        _vChain.push_back(block);
    }
    //TODO for test
    _nIndex=root.size();
    GetJsonString();

    ifs.close();
    if (!IsValidate()) {
        std::cout << " the data was not valid" << std::endl;
        exit(1);
    }
    return;
}
//save the block chain data in to file in JSON format
void  Blockchain::Save(std::string file) {

    std::ofstream ofs;
    ofs.open(file);
    if (!ofs.is_open())
    {
        std::cout << "open json file failed." << std::endl;
        return;
    }
    ofs << GetJsonString();
    ofs.close();
}
//used for increment save the block
void  Blockchain::SaveOneBlock(std::string fileName, Block block)  {


}
//API to read all the data from block chain in JSON format
std::string Blockchain::GetJsonString(void)
{
    std::string jsonStr;
    Json::Value root,chain;
    Json::StreamWriterBuilder writerBuilder;
    std::ostringstream os;
    //auto rewind
    writerBuilder.settings_["indentation"] = "\t";

    for (int i=0;i<_vChain.size();i++)
    {
       chain= _vChain[i].toJsonObj();
       root[i]=chain;
    }

    std::unique_ptr<Json::StreamWriter> jsonWriter(writerBuilder.newStreamWriter());
    jsonWriter->write(root, &os);
    jsonStr = os.str();

    std::cout << "block chains:" << jsonStr << std::endl;
    return jsonStr;
}