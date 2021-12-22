#include "Blockchain.h"

#include <fstream>
int main()
{
    std::unique_ptr<Blockchain>  bChain( new Blockchain());

    cout << "Mining block 1..." << endl;
//    bChain.AddBlock(Block(1, "Block 1 Data"));

//    bChain.Load("../json/blockchain.json");
//    bChain.Save("../json/blockchain.json");


//    cout << "Mining block 2..." << endl;
//    bChain.AddBlock(Block(2, "Block 2 Data"));
//
//    cout << "Mining block 3..." << endl;
//    bChain.AddBlock(Block(3, "Block 3 Data"));
    while(true);
    return 0;
}
