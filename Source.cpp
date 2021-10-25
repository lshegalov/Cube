// 
//

#include <memory>

class myString
{
private:
    char * startS = nullptr;

    size_t length = 0;

    myString();
    ~myString();

    myString & operator =(const myString& rhs) 
    {
        if (this == &rhs)
        {
            return *this;
        }

        char * pTemp = new char[length + 1];

        delete [] startS;

        startS = pTemp;

        length = rhs.length;

        memcpy(startS, rhs.startS, length + 1);

        return *this;
    }

   
};

