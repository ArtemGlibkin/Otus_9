#include <string>
#include "async.h"
class Commutator
{
    int mCommonHandler;
    int mOwnHandler = -1;
    int mBlockSize = 1;
    bool dynamic_block = false;

    int getOwnHandler()
    {
        
        if(mOwnHandler == -1)
            mOwnHandler = connect_bulk(mBlockSize);
            
        return mOwnHandler;
    }
    
public:
    Commutator(int common_handler, int block_size)   : mCommonHandler(common_handler), mBlockSize(block_size)
    {

    }
    
    void push(const std::string & str)
    {
        if(!dynamic_block && (str.find("{") != std::string::npos))
            dynamic_block = true;

        if(dynamic_block)
            receive(getOwnHandler(), str.c_str(), str.size());
        else
            receive(mCommonHandler, str.c_str(), str.size());
        
        if(dynamic_block && (str.find("}") != std::string::npos))
            dynamic_block = false;
    }

    ~Commutator()
    {
        if(mOwnHandler != -1)
            disconnect(mOwnHandler);
    }
};