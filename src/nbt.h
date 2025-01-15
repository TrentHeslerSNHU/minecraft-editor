#include <string>
#include <iostream>
#include <string.h>
#include <variant>
#include <vector>

#ifndef NBT_H
#define NBT_H

enum tagType {
    EndTag,
    ByteTag,
    ShortTag,
    IntTag,
    LongTag,
    FloatTag,
    DoubleTag,
    ByteArrayTag,
    StringTag,
    ListTag,
    CompoundTag
};

//Abstract class
class NbtTag {
    public:
        short *getType(){return this->TagType;}
        NbtTag* getNext(){return this->Next;}
        void setNext(NbtTag *newNext){this->Next = newNext;}
        std::string *getName(){return this->Name;}
    protected:
        short *TagType = nullptr;
        std::string *Name = nullptr;
        NbtTag *Prev = nullptr;
        NbtTag *Next = nullptr;
};

//TYPE 0: End
class TagEnd : public NbtTag {
    public:
        TagEnd(){this->TagType=new short(0); this->Name=new std::string("");}
};

//TYPE 1: Byte
class TagByte : public NbtTag {
    public:
        TagByte(){this->TagType = new short(1); this->Name=new std::string("TagByte");}
        TagByte(const std::string, unsigned char);
    private:
        unsigned char *Payload;
};

//TYPE 2: Short
class TagShort : public NbtTag {
    public:
        TagShort(){this->TagType = new short(2); this->Name=new std::string("TagShort");}
        TagShort(const std::string, short);
    private:
        short *Payload;
};

//TYPE 3: Int
class TagInt : public NbtTag {
    public:
        TagInt(){this->TagType = new short(3); this->Name=new std::string("TagInt");}
        TagInt(const std::string, int);
    private:
        int *Payload;
};

//TYPE 4: Long
class TagLong : public NbtTag {
    public:
        TagLong(){this->TagType = new short(4); this->Name=new std::string("TagLong");}
        TagLong(const std::string, long);
    private:
        long *Payload;
};

//TYPE 5: Float
class TagFloat : public NbtTag {
    public:
        TagFloat(){this->TagType = new short(5); this->Name=new std::string("TagFloat");}
        TagFloat(const std::string, float);
    private:
        float *Payload;
};

//TYPE 6: Double
class TagDouble : public NbtTag {
    public:
        TagDouble(){this->TagType = new short(6); this->Name=new std::string("TagDouble");}
        TagDouble(const std::string, double);
    private:
        double *Payload;
};

//TYPE 7: Byte Array
class TagByteArray : public NbtTag {
    public:
        TagByteArray(){this->TagType = new short(7); this->Name=new std::string("TagByteArray");}
        TagByteArray(const std::string);
    private:
        int *Length;
        unsigned char *Payload;
};

//TYPE 8: String
class TagString : public NbtTag {
    public:
        TagString(){this->TagType = new short(8); this->Name=new std::string("TagString");}
        TagString(const std::string, const std::string);
        std::string getPayload(){return *Payload;}
    private:
        std::string *Payload = nullptr;
};

//TYPE 9: List
class TagList : public NbtTag {
    //TODO: Refactor entire class
    private:
        //Maybe you have this backwards? Instead of vector of variants, maybe variant of vectors?
        typedef std::variant<unsigned char,short,int,long,float,double,std::string> listItem;
        unsigned char *ItemType;
        int *Qty;
        std::vector<listItem> *Payload = nullptr;
    public:
        TagList(){this->TagType = new short(9); this->Name=new std::string("TagList"); this->Payload=new std::vector<listItem>();}
        TagList(const std::string, unsigned char, int);
        void addListItem(listItem);
        void printItems();
        int listAmt();
};

//TYPE 10: Compound
class TagCompound : public NbtTag {
    public:
        TagCompound(){this->TagType = new short(10); this->Name=new std::string("TagCompound");}
        TagCompound(const std::string);
        void appendChild(NbtTag *);
        void listChildren();
        int childAmt();
    private:
        NbtTag *Parent = nullptr;
        NbtTag *Children = nullptr;
};

//TODO: Add types 11 and 12 (IntArray and LongArray)

/*
Class member function definitions
*/

void TagCompound::appendChild(NbtTag *NewChild){
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
            std::cout << "[" << *(nextTag->getType()) << "]: ";
            if(*(nextTag->getType()) != 0){
                std::cout << *(nextTag->getName());
                if(*(nextTag->getType()) == 9){
                    TagList *sharts = static_cast<TagList *>(nextTag);
                    std::cout << "(" << sharts->listAmt() << ")";
                } else if (*(nextTag->getType()) == 10) {
                    TagCompound *myPtr = static_cast<TagCompound *>(nextTag);
                    std::cout << "(" << myPtr->childAmt() << ")";
                }
                std::cout << std::endl;
            } else {
                std::cout << "[END]" << std::endl;
            }
            nextTag = nextTag->getNext();
        }
    }
}

int TagCompound::childAmt(){
    int i = 0;
    if(this->Children != nullptr){
        NbtTag *nextTag = this->Children;
        
        while(nextTag != nullptr){
            i++;
            if(*(nextTag->getType()) == 10){
                TagCompound *farts = static_cast<TagCompound *>(nextTag);
                i+=farts->childAmt();
                std::cout << *(farts->getName()) << std::endl;
                farts->listChildren();
            }
            nextTag = nextTag->getNext();
        }
    }
    return i;
}

void TagList::addListItem(listItem NewItem){
    if(Payload == nullptr){
        Payload = new std::vector<listItem>();
    }
    Payload->push_back(NewItem);
}

int TagList::listAmt(){
    return Payload->size();
}

void TagList::printItems(){
    if(Payload != nullptr){
        for(int i=0; i < Payload->size(); i++){
            listItem currentItem = Payload->at(i);
            switch (currentItem.index())
            {
            case 0:
            {
                std::cout << "BYTE: " << std::get<unsigned char>(currentItem) << std::endl;
                break;
            }
            
            case 1:
            {
                std::cout << "SHORT: " << std::get<short>(currentItem) << std::endl;
                break;
            }

            case 2:
            {
                std::cout << "INT: " << std::get<int>(currentItem) << std::endl;
                break;
            }

            case 3:
            {
                std::cout << "LONG: " << std::get<long>(currentItem) << std::endl;
                break;
            }

            case 4:
            {
                std::cout << "FLOAT: " << std::get<float>(currentItem) << std::endl;
                break;
            }

            case 5:
            {
                std::cout << "DOUBLE: " << std::get<double>(currentItem) << std::endl;
                break;
            }

            case 6:
            {
                std::cout << "STRING: " << std::get<std::string>(currentItem) << std::endl;
                break;
            }

            default:
            {break;}
            }
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
    this->ItemType = new unsigned char(ListType);
    this->Qty = new int(NumItems);
    this->Payload = new std::vector<listItem>();
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

//============ TagBuilders ============
TagByte *getByte(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    //std::cout << "BYTE: " << Name << " = " << Data[i]/1 << std::endl;
    TagByte *Tag = new TagByte(Name,Data[i]);
    return Tag;
}

TagShort *getShort(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    //std::cout << "SHORT: " << Name << " = " << convertToShort(Data.substr(i,2)) << std::endl;
    TagShort *Tag = new TagShort(Name,convertToShort(Data.substr(i,2)));
    i++;
    return Tag;
}

TagInt *getInt(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    //std::cout << "INT: " << Name << " = " << convertToInt(Data.substr(i,4)) << std::endl;
    TagInt *Tag = new TagInt(Name,convertToInt(Data.substr(i,4)));
    i+=3;
    return Tag;
}

TagLong *getLong(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    //std::cout << "LONG: " << Name << " = " << convertToLong(Data.substr(i,8)) << std::endl;
    TagLong *Tag = new TagLong(Name,convertToLong(Data.substr(i,8)));
    i+=7;
    return Tag;
}

TagFloat *getFloat(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    //std::cout << "FLOAT: " << Name << " = " << convertToFloat(Data.substr(i,4)) << std::endl;
    TagFloat *Tag = new TagFloat(Name,convertToFloat(Data.substr(i,4)));
    i+=3;
    return Tag;
}

TagDouble *getDouble(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    //std::cout << "DOUBLE: " << Name << " = " << convertToDouble(Data.substr(i,8)) << std::endl;
    TagDouble *Tag = new TagDouble(Name,convertToDouble(Data.substr(i,8)));
    return Tag;
}

TagByteArray *getByteArray(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    TagByteArray *Tag = new TagByteArray();
    return Tag;
}

TagString *getString(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    int PayloadLen = int(Data[i]);
    i+=2;
    //std::cout << "STRING: " << Name << " = \"" << Data.substr(i,PayloadLen) << "\"" << std::endl;
    TagString *Tag = new TagString(Name,Data.substr(i,PayloadLen));
    i+=PayloadLen-1;
    return Tag;
}

TagList *getList(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    int itemType = int(Data[i]);
    int itemQty = int(Data[i+1]);
    i+=4;

    //std::cout << "LIST: " << Name << " (" << itemQty << " item(s) of type " << itemType << ")" << std::endl;
    TagList *Tag = new TagList(Name,itemType,itemQty);
    
    switch (itemType)
    {
    case ByteTag:
    {
        for (size_t j = 0; j < itemQty; j++)
        {
            //std::cout << "⊦BYTE: " << Data[i+j]/1 << std::endl;
            Tag->addListItem(Data[i+j]);
        }
        i+=itemQty;
        break;
    }

    case ShortTag:
    {
        for (size_t j = 0; j < itemQty; j++)
        {
            //std::cout << "⊦SHORT: " << convertToShort(Data.substr(i+(j*2),2)) << std::endl;
            Tag->addListItem(convertToShort(Data.substr(i+(j*2),2)));
        }
        i+=(itemQty*2);
        break;
    }
    
    case IntTag:
    {
        for (size_t j = 0; j < itemQty; j++)
        {
            //std::cout << "⊦INT: " << convertToInt(Data.substr(i+(j*4),4)) << std::endl;
            Tag->addListItem(convertToInt(Data.substr(i+(j*4),4)));
        }
        i+=(itemQty*4);
        break;
    }

    case LongTag:
    {
        for (size_t j = 0; j < itemQty; j++)
        {
            //std::cout << "⊦LONG: " << convertToLong(Data.substr(i+(j*8),8)) << std::endl;
            Tag->addListItem(convertToLong(Data.substr(i+(j*8),8)));
        }
        i+=(itemQty*8);
        break;
    }

    case FloatTag:
    {
        i+=3;
        for (size_t j = 0; j < itemQty; j++)
        {
            //std::cout << "⊦FLOAT: " << convertToFloatMBE(Data.substr(i+(j*4),4)) << std::endl;
            Tag->addListItem(convertToFloatMBE(Data.substr(i+(j*4),4)));
        }
        i-=3;
        i+=(itemQty*4);
        break;
    }

    case DoubleTag:
    {
        for (size_t j = 0; j < itemQty; j++)
        {
            //std::cout << "⊦DOUBLE: " << convertToDouble(Data.substr(i+(j*8),8)) << std::endl;
            Tag->addListItem(convertToDouble(Data.substr(i+(j*8),8)));
        }
        i+=(itemQty*8);
        break;
    }

    case ByteArrayTag:
    {
        //FIXME: Fill this in
        break;
    }

    case StringTag:
    {
        i++;
        for(size_t k=0; k<itemQty; k++)
        {
            int strSize = int(Data[i]);
            i+=2;
            //std::cout << "⊦STRING: \"" << Data.substr(i,strSize) << "\"" << std::endl;
            Tag->addListItem(Data.substr(i,strSize));
            i+=strSize;
        }
        i--;
        break;
    }

    case CompoundTag:
    {
        //FIXME: Add logic for CompoundTag
        //This will require completely redoing the ListTag class. Yay.
    }

    case EndTag:
    {
        //FIXME: Should there be something here?
        break;
    }

    default:
        std::cout << "Unknown (" << itemType << ")" << std::endl;
        break;
    }

    return Tag;
}

TagCompound *getCompound(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    TagCompound *root = new TagCompound(Name);
    i+=nlen;
    //std::cout << "COMPOUND: " << Name << std::endl;

    for(i; i<Data.length(); i++){
        short typeCode = short(Data[i]);
        i++;

        switch (typeCode)
        {
        case EndTag://0
        {
            root->appendChild(new TagEnd());
            i--;
            return root;
        }
        
        case ByteTag://1
        {
            //std::cout << "BYTE:";
            root->appendChild(getByte(Data,i));
            break;
        }

        case ShortTag://2
        {
            //std::cout << "SHORT:";
            root->appendChild(getShort(Data,i));
            break;
        }

        case IntTag://3
        {
            //std::cout << "INT:";
            root->appendChild(getInt(Data,i));
            break;
        }

        case LongTag://4
        {
            //std::cout << "LONG:";
            root->appendChild(getLong(Data,i));
            break;
        }

        case FloatTag://5
        {
            //std::cout << "FLOAT:";
            root->appendChild(getFloat(Data,i));
            break;
        }

        case DoubleTag://6
        {
            //std::cout << "DOUBLE:";
            root->appendChild(getDouble(Data,i));
            break;
        }

        case ByteArrayTag://7
        {
            //std::cout << "BYTEARRAY:";
            root->appendChild(getByteArray(Data,i));
            break;
        }

        case StringTag://8
        {
            //std::cout << "STRING:";
            root->appendChild(getString(Data,i));
            break;
        }

        case ListTag://9
        {
            //std::cout << "LIST:";
            root->appendChild(getList(Data,i));
            break;
        }

        case CompoundTag://10
        {
            //std::cout << "COMPOUND:";
            root->appendChild(getCompound(Data,i));
            break;
        }

        default:
        {
            std::cout << "[" << typeCode << "]";
            break;
        }
       }
    }
    
    return root;
}

//======================= The "Main Event" =======================

TagCompound getNBT(const std::string Data){
    TagCompound *root = new TagCompound("root");
    for(int i=3; i<Data.length(); i++){
        short typeCode = short(Data[i]);
        i++;
        
        switch (typeCode)
        {
        case EndTag://0
        {
            //std::cout << "END" << std::endl;
            root->appendChild(new TagEnd());
            i--;
            break;
        }
        
        case ByteTag://1
        {
            //std::cout << "BYTE:";
            root->appendChild(getByte(Data,i));
            break;
        }

        case ShortTag://2
        {
            //std::cout << "SHORT:";
            root->appendChild(getShort(Data,i));
            break;
        }

        case IntTag://3
        {
            //std::cout << "INT:";
            root->appendChild(getInt(Data,i));
            break;
        }

        case LongTag://4
        {
            //std::cout << "LONG:";
            root->appendChild(getLong(Data,i));
            break;
        }

        case FloatTag://5
        {
            //std::cout << "FLOAT:";
            root->appendChild(getFloat(Data,i));
            break;
        }

        case DoubleTag://6
        {
            //std::cout << "DOUBLE:";
            root->appendChild(getDouble(Data,i));
            break;
        }

        case ByteArrayTag://7
        {
            //std::cout << "BYTEARRAY:";
            root->appendChild(getByteArray(Data,i));
            break;
        }

        case StringTag://8
        {
            //std::cout << "STRING:";
            root->appendChild(getString(Data,i));
            break;
        }

        case ListTag://9
        {
            //std::cout << "LIST:";
            root->appendChild(getList(Data,i));
            break;
        }

        case CompoundTag://10
        {
            //std::cout << "COMPOUND:";
            root->appendChild(getCompound(Data,i));
            break;
        }

        default:
        {
            std::cout << "[" << typeCode << "]";
            break;
        }
       }
    }
    
    return *root;
}

#endif