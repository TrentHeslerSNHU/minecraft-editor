#include "nbt.h"

bool PRINT_PARSE=true;
bool PRINT_TAG_BOUNDS=true;

/*
Class member function definitions
*/

void TagCompound::addChild(NbtTag *NewChild){
    if(this->Children != nullptr){
        NbtTag *LastChild = this->Children;
        while(LastChild->getNext() != nullptr){
            LastChild = LastChild->getNext();
        }
        LastChild->setNext(NewChild);
    } else {
        this->Children = NewChild;
    }
}

void TagCompound::listChildren() {
    if(this->Children == nullptr){
        std::cout << "This tag has no children." << std::endl;
    } else {
        int i = 0;
        NbtTag *nextTag = this->Children;
        while(nextTag != nullptr){
            i++;
            std::cout << "[" << nextTag->getType() << "]: ";
            if(nextTag->getType() != 0){
                std::cout << nextTag->getName();
                if(nextTag->getType() == 9){
                    TagList *lstPtr = static_cast<TagList *>(nextTag);
                    std::cout << "(" << lstPtr->numChildren() << ")";
                    //sharts->listChildren();
                } else if (nextTag->getType() == 10) {
                    TagCompound *cmpPtr = static_cast<TagCompound *>(nextTag);
                    std::cout << "(" << cmpPtr->numChildren() << ")";
                    //myPtr->listChildren();
                }
                std::cout << std::endl;
            } else {
                std::cout << "[END]" << std::endl;
            }
            nextTag = nextTag->getNext();
        }
    }
}

int TagCompound::numChildren(){
    int i = 0;
    if(this->Children != nullptr){
        NbtTag *nextTag = this->Children;
        
        while(nextTag != nullptr){
            i++;
            if(nextTag->getType() == 10){
                TagCompound *farts = static_cast<TagCompound *>(nextTag);
                i+=farts->numChildren();
            }
            nextTag = nextTag->getNext();
        }
    }
    return i;
}

void TagList::addChild(NbtTag *NewItem){
    if(this->Children != nullptr){
        NbtTag *LastChild = this->Children;
        while(LastChild->getNext() != nullptr){
            LastChild = LastChild->getNext();
        }
        LastChild->setNext(NewItem);
    } else {
        this->Children = NewItem;
    }
}

int TagList::numChildren(){
int i = 0;
    if(this->Children != nullptr){
        NbtTag *nextTag = this->Children;
        
        while(nextTag != nullptr){
            i++;
            if(nextTag->getType() == 10){
                TagCompound *farts = static_cast<TagCompound *>(nextTag);
                i+=farts->numChildren();
                i--;
            }
            nextTag = nextTag->getNext();
        }
    }
    return i;
}

void TagList::listChildren(){
    if(Children != nullptr){
        NbtTag *currentItem = this->Children;
        while(currentItem != nullptr){
            switch (currentItem->getType())
            {
            case 1:
            {
                std::cout << "BYTE: " << currentItem->getName() << std::endl;
                break;
            }
            
            case 2:
            {
                std::cout << "SHORT: " << currentItem->getName() << std::endl;
                break;
            }

            case 3:
            {
                std::cout << "INT: " << currentItem->getName() << std::endl;
                break;
            }

            case 4:
            {
                std::cout << "LONG: " << currentItem->getName() << std::endl;
                break;
            }

            case 5:
            {
                std::cout << "FLOAT: " << currentItem->getName() << std::endl;
                break;
            }

            case 6:
            {
                std::cout << "DOUBLE: " << currentItem->getName() << std::endl;
                break;
            }

            case 7:
            {
                std::cout << "BYTEARRAY: " << currentItem->getName() << std::endl;
                break;
            }

            case 8:
            {
                std::cout << "STRING: " << currentItem->getName() << std::endl;
                break;
            }

            case 9:
            {
                std::cout << "LIST: " << std::endl;
                break;
            }

            case 10:
            {
                std::cout << "COMPOUND: " << currentItem->getName() << "(" << ((TagCompound *)currentItem)->numChildren() << ")" << std::endl;
                //((TagCompound *)currentItem)->listChildren();
                break;
            }
            //TODO: Add compound (and eventually types 11 and 12 (IntArray and LongArray))

            default:
            {break;}
            }
            currentItem = currentItem->getNext();
        }
    }
}

TagByte::TagByte(const std::string NewName, unsigned char NewPayload)
{
    this->Name = new std::string(NewName);
    this->TagType = new short(1);
    this->Payload = new unsigned char(NewPayload);
}

TagShort::TagShort(const std::string NewName, short NewPayload)
{
    this->Name = new std::string(NewName);
    this->TagType = new short(2);
    this->Payload = new short(NewPayload);
}

TagInt::TagInt(const std::string NewName, int NewPayload)
{
    this->Name = new std::string(NewName);
    this->TagType = new short(3);
    this->Payload = new int(NewPayload);
}

TagLong::TagLong(const std::string NewName, long NewPayload)
{
    this->Name = new std::string(NewName);
    this->TagType = new short(4);
    this->Payload = new long(NewPayload);
}

TagFloat::TagFloat(const std::string NewName, float NewPayload)
{
    this->Name = new std::string(NewName);
    this->TagType = new short(5);
    this->Payload = new float(NewPayload);
}

TagDouble::TagDouble(const std::string NewName, double NewPayload)
{
    this->Name = new std::string(NewName);
    this->TagType = new short(6);
    this->Payload = new double(NewPayload);
}

TagByteArray::TagByteArray(const std::string NewName)
{
    this->Name = new std::string(NewName);
    this->TagType = new short(7);
    //this->Payload = new unsigned char[](*NewPayload);
}

TagString::TagString(const std::string NewName, std::string NewPayload)
{
    this->Name = new std::string( NewName);
    this->TagType = new short(8);
    this->Payload = new std::string(NewPayload);
}

TagList::TagList(const std::string NewName, unsigned char ListType, int NumItems)
{
    this->Name = new std::string(NewName);
    this->TagType = new short(9);
    this->itemType = new unsigned char(ListType);
    this->itemQty = new int(NumItems);
}

TagCompound::TagCompound(const std::string NewName){
    this->Name = new std::string(NewName);
    this->TagType = new short(10);
}

//======================= Helper Functions =======================

short convertToShort(const std::string data){
    short val = 0;
    unsigned char bytes[2] = {data[0],data[1]};
    memcpy(&val,bytes,2);
    return val;
}

int convertToInt(const std::string data){
    int val = 0;
    unsigned char bytes[4] = {data[0],data[1],data[2],data[3]};
    memcpy(&val,bytes,4);
    return val;
}

long convertToLong(const std::string data){
    long val = 0;
    unsigned char bytes[8] = {data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
    memcpy(&val,bytes,8);
    return val;
}

float convertToFloat(const std::string data){
    float val = 0;
    unsigned char bytes[4] = {data[0],data[1],data[2],data[3]};
    memcpy(&val,bytes,4);
    return val;
}

float convertToFloatMBE(const std::string data){
    float val = 0;
    unsigned char bytes[] = {data[2],data[3],data[0],data[1]};
    memcpy(&val,bytes,sizeof(float));
    return val;
}

double convertToDouble(const std::string data){
    double val = 0;
    unsigned char bytes[8] = {data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]};
    memcpy(&val,bytes,8);
    return val;
}

//=======================   TagBuilders   =======================
TagByte *getByte(const std::string &Data, int &i) {
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }
    i++;
    int nlen = Data[i];
    i+=2;
    std::string name = Data.substr(i,nlen);
    i+=nlen;
    if(PRINT_PARSE){
        std::cout << "BYTE: " << name << " = " << ((unsigned char)Data[i])/1 << "\n";
    }
    if(PRINT_TAG_BOUNDS){
        std::cout << "</" << i << ">\n";
    }
    return new TagByte(name,Data[i]);
}
TagShort *getShort(const std::string &Data, int &i) {
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }
    i++;
    int nlen = Data[i];
    i+=2;
    std::string name = Data.substr(i,nlen);
    i+=nlen;
    if(PRINT_PARSE){
        std::cout << "SHORT: " << name << " = " << convertToShort(Data.substr(i,2)) << "\n";
    }
    if(PRINT_TAG_BOUNDS){
        std::cout << "</" << i << ">\n";
    }
    return new TagShort(name,convertToShort(Data.substr(i,2)));
}
TagInt *getInt(const std::string &, int &i) {}
TagLong *getLong(const std::string &, int &) {}
TagFloat *getFloat(const std::string &, int &) {}
TagDouble *getDouble(const std::string &, int &) {}
TagByteArray *getByteArray(const std::string &, int &) {}
TagString *getString(const std::string &, int &) {}
TagList *getList(const std::string &, int &) {}
TagCompound *getCompound(const std::string &, int &) {}
TagCompound *getUnnamedCompound(const std::string &, int &) {}

//======================= The "Main Event" =======================
TagCompound getNBT(const std::string Data) {
    TagCompound *root = new TagCompound("root");

    for(int i=3; i<Data.length(); i++){
        short TagType = Data[i];

        switch (TagType)
        {
        case ByteTag:
        {
            root->addChild(getByte(Data,i));
            break;
        }
        
        case ShortTag:
        {
            root->addChild(getShort(Data,i));
            break;
        }

        case IntTag:
        {
            root->addChild(getInt(Data,i));
            break;
        }

        case LongTag:
        {
            root->addChild(getLong(Data,i));
            break;
        }

        case FloatTag:
        {
            root->addChild(getFloat(Data,i));
            break;
        }

        case DoubleTag:
        {
            root->addChild(getDouble(Data,i));
            break;
        }
        
        case ByteArrayTag:
        {
            root->addChild(getByteArray(Data,i));
            break;
        }

        case StringTag:
        {
            root->addChild(getString(Data,i));
            break;
        }

        case ListTag:
        {
            root->addChild(getList(Data,i));
            break;
        }

        case CompoundTag:
        {
            root->addChild(getCompound(Data,i));
            break;
        }

        case EndTag:
        {
            root->addChild(new TagEnd());
            return *root;
        }

        default:
            break;
        }
    }
}