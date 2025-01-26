#include "nbt.h"

bool PRINT_PARSE=true;
bool PRINT_TAG_BOUNDS=true;

/*********************
 * Current Progress: *
 * Create: ✘         *
 *   Read: ✔         *
 * Update: ✘         *
 * Delete: ✘         *
 ********************/

// Next order of business: get ByteArrays figured out
// Second order of business: make a function to re-print tags as NBT string
// (This is the first step towards creating, deleting, and updating)

//TODO: (Eventually) Remove printed output from parse functions

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
                    std::cout << "(" << lstPtr->numChildren() << ")\n";
                    lstPtr->listChildren();
                } else if (nextTag->getType() == 10) {
                    TagCompound *cmpPtr = static_cast<TagCompound *>(nextTag);
                    std::cout << "(" << cmpPtr->numChildren() << ")\n";
                    cmpPtr->listChildren();
                } else {
                    std::cout << " = " << nextTag->getPayload() << std::endl;
                }
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
                TagCompound *cmpPtr = static_cast<TagCompound *>(nextTag);
                i+=cmpPtr->numChildren();
            } else if (nextTag->getType() == 9) {
                TagList *listPtr = static_cast<TagList *>(nextTag);
                i+=listPtr->numChildren();
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
                TagCompound *cmpPtr = static_cast<TagCompound *>(nextTag);
                i+=cmpPtr->numChildren();
            } else if (nextTag->getType() == 9){
                TagList *listPtr = static_cast<TagList *>(nextTag);
                i+=listPtr->numChildren();
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
                std::cout << "BYTE: " << currentItem->getName();
                std::cout << " = " << currentItem->getPayload() << std::endl;
                break;
            }
            
            case 2:
            {
                std::cout << "SHORT: " << currentItem->getName();
                std::cout << " = " << currentItem->getPayload() << std::endl;
                break;
            }

            case 3:
            {
                std::cout << "INT: " << currentItem->getName();
                std::cout << " = " << currentItem->getPayload() << std::endl;
                break;
            }

            case 4:
            {
                std::cout << "LONG: " << currentItem->getName();
                std::cout << " = " << currentItem->getPayload() << std::endl;
                break;
            }

            case 5:
            {
                std::cout << "FLOAT: " << currentItem->getName();
                std::cout << " = " << currentItem->getPayload() << std::endl;
                break;
            }

            case 6:
            {
                std::cout << "DOUBLE: " << currentItem->getName();
                std::cout << " = " << currentItem->getPayload() << std::endl;
                break;
            }

            case 7:
            {
                std::cout << "BYTEARRAY: " << currentItem->getName();
                std::cout << " = " << currentItem->getPayload() << std::endl;
                break;
            }

            case 8:
            {
                std::cout << "STRING: " << currentItem->getName();
                std::cout << " = " << currentItem->getPayload() << std::endl;
                break;
            }

            case 9:
            {
                std::cout << "LIST: " << currentItem->getName() << "(" << ((TagList *)currentItem)->numChildren() << ")" << std::endl;
                ((TagList *)currentItem)->listChildren();
                break;
            }

            case 10:
            {
                std::cout << "COMPOUND: " << currentItem->getName() << "(" << ((TagCompound *)currentItem)->numChildren() << ")" << std::endl;
                ((TagCompound *)currentItem)->listChildren();
                break;
            }
            //TODO: Eventually add types 11 and 12 (IntArray and LongArray)

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
    std::string name = Data.substr(i,nlen+1);
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
    std::string name = Data.substr(i,nlen+1);
    i+=nlen;
    if(PRINT_PARSE){
        std::cout << "SHORT: " << name << " = " << convertToShort(Data.substr(i,2)) << "\n";
    }
    i+=sizeof(short)-1;
    if(PRINT_TAG_BOUNDS){
        std::cout << "</" << i << ">\n";
    }
    return new TagShort(name,convertToShort(Data.substr(i,2)));
}

TagInt *getInt(const std::string &Data, int &i) {
        if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }
    i++;
    int nlen = Data[i];
    i+=2;
    std::string name = Data.substr(i,nlen+1);
    i+=nlen;
    int payload = convertToInt(Data);
    if(PRINT_PARSE){
        std::cout << "INT: " << name << " = " << payload << "\n";
    }
    i+=sizeof(int)-1;
    if(PRINT_TAG_BOUNDS){
        std::cout << "</" << i << ">\n";
    }
    return new TagInt(name,payload);
}

TagLong *getLong(const std::string &Data, int &i) {
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }
    i++;
    int nlen = Data[i];
    i+=2;
    std::string name = Data.substr(i,nlen+1);
    i+=nlen;
    long payload = convertToLong(Data);
    if(PRINT_PARSE){
        std::cout << "LONG: " << name << " = " << payload << "\n";
    }
    i+=sizeof(long)-1;
    if(PRINT_TAG_BOUNDS){
        std::cout << "</" << i << ">\n";
    }
    return new TagLong(name,payload);
}

TagFloat *getFloat(const std::string &Data, int &i) {
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }
    i++;
    int nlen = Data[i];
    i+=2;
    std::string name = Data.substr(i,nlen+1);
    i+=nlen;
    float payload = convertToFloat(Data);
    if(PRINT_PARSE){
        std::cout << "FLOAT: " << name << " = " << payload << "\n";
    }
    i+=sizeof(float)-1;
    if(PRINT_TAG_BOUNDS){
        std::cout << "</" << i << ">\n";
    }
    return new TagFloat(name,payload);
}

TagDouble *getDouble(const std::string &Data, int &i) {
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }
    i++;
    int nlen = Data[i];
    i+=2;
    std::string name = Data.substr(i,nlen+1);
    i+=nlen;
    double payload = convertToDouble(Data);
    if(PRINT_PARSE){
        std::cout << "DOUBLE: " << name << " = " << payload << "\n";
    }
    i+=sizeof(double)-1;
    if(PRINT_TAG_BOUNDS){
        std::cout << "</" << i << ">\n";
    }
    return new TagDouble(name,payload);
}

//FIXME: Redo this after researching bytearray tags
TagByteArray *getByteArray(const std::string &Data, int &i) {
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }
    i++;
    int nlen = Data[i];
    i+=2;
    std::string name = Data.substr(i,nlen+1);
    i+=nlen;
    //long payload = convertToLong(Data);
    if(PRINT_PARSE){
        std::cout << "BYTEARRAY: " << name << " = " << /*payload <<*/ "\n";
    }
    i+=sizeof(long)-1;
    if(PRINT_TAG_BOUNDS){
        std::cout << "</" << i << ">\n";
    }
    //return new TagLong(name,payload);
    return new TagByteArray(name);
}

TagString *getString(const std::string &Data, int &i) {
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }
    i++;
    int nlen = Data[i];   
    i+=2;
    std::string name = Data.substr(i,nlen+1);
    i+=nlen;
    int strlen = Data[i];
    i++;
    std::string payload = "";
    if(strlen != 0){
        payload = Data.substr(i,strlen+1);
        i+=sizeof(payload)-1;
    }
    if(PRINT_PARSE){
        std::cout << "STRING: " << name << " = \"" << payload << "\"\n";
    }
    if(PRINT_TAG_BOUNDS){
        std::cout << "</" << i << ">\n";
    }
    return new TagString(name,payload);
}

TagList *getList(const std::string &Data, int &i) {
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }
    i++;
    int nlen = Data[i];
    i+=2;
    std::string name = Data.substr(i,nlen+1);
    i+=nlen;
    
    int itemType = Data[i];
    i++;
    int itemQty = Data[i];
    i+=4;

    if(PRINT_PARSE){
        std::cout << "LIST: " << name << " ("<< itemQty << " tags of type " << itemType <<")\n";
    }
    
    TagList *root = new TagList(name,itemType,itemQty);

    //TODO: Fill this in
    for (int j=0; j < itemQty; j++){
        switch (itemType)
        {
        
        case ByteTag:
        {
            
            break;
        }

        case ShortTag:
        {
            break;
        }
        
        case IntTag:
        {
            break;
        }

        case LongTag:
        {
            break;
        }

        case FloatTag:
        {
            break;
        }

        case DoubleTag:
        {
            break;
        }

        case ByteArrayTag:
        {
            break;
        }

        case StringTag:
        {
            break;
        }

        case ListTag:
        {
            std::cout << "How meta.\n";
            break;
        }

        case CompoundTag:
        {
            root->addChild(getUnnamedCompound(Data,i));
            break;
        }

        default:
            break;
        }
    }

    i--;
    return root;
}

TagCompound *getCompound(const std::string &Data, int &i) {
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }

    i++;
    int nlen = Data[i];
    i+=2;
    std::string name = Data.substr(i,nlen+1);
    i+=nlen;

    TagCompound *root = new TagCompound();

    for(i; i<Data.size(); i++){
        int itemType = Data[i];
        switch (itemType)
        {
        case EndTag:
        {
            root->addChild(getEnd(i));
            return root;
            break;
        }

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

        case CompoundTag:
        {
            root->addChild(getCompound(Data,i));
            break;
        }

        case ListTag:
        {
            root->addChild(getList(Data,i));
            break;
        }

        default:
            break;
        }
    }

    return root;
}

TagCompound *getUnnamedCompound(const std::string &Data, int &i) {
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }

    TagCompound *root = new TagCompound();

    for(i; i<Data.size(); i++){
        int itemType = Data[i];
        switch (itemType)
        {
        case EndTag:
        {
            root->addChild(getEnd(i));
            return root;
            break;
        }

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

        case CompoundTag:
        {
            root->addChild(getCompound(Data,i));
            break;
        }

        case ListTag:
        {
            root->addChild(getList(Data,i));
            break;
        }

        default:
            break;
        }
    }

    return root;
}

TagEnd *getEnd(int &i){
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }
    if(PRINT_PARSE){
        std::cout << "[END]\n";
    }
    i++;
    if(PRINT_TAG_BOUNDS){
        std::cout << "</" << i << ">\n";
    }
    return new TagEnd();
}

//======================= The "Main Event" =======================
TagCompound getNBT(const std::string Data) {
    TagCompound *root = new TagCompound("root");

    for(int i=3; i<Data.length(); i++){
        short TagType = Data[i];

        switch (TagType)
        {
        
        case EndTag:
        {
            root->addChild(getEnd(i));
            return *root;
            break;
        }
        
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

        default:
        {
            std::cout << "What's this? A tag of type " << TagType << " at " << i << "? Gasp!\n";
            exit(1);
            break;
        }
        }
    }
}

TagByte *parseByte(const std::string &data, int &i)
{
    i++;
    int nlen = int(data[i]);
    i+=2;
    std::string name = data.substr(i,nlen);
    i+=nlen;
    unsigned char payload = (unsigned char)data[i];
    std::cout << "BYTE: " << name << " = " << payload/1 << std::endl;
    return new TagByte(name,payload);
}

TagShort *parseShort(const std::string &data, int &i)
{
    i++;
    int nlen = int(data[i]);
    i+=2;
    std::string name = data.substr(i,nlen);
    i+=nlen;
    short shortPayload=convertToShort(data.substr(i,2));
    std::cout << "SHORT: " << name << " = " << shortPayload << std::endl;
    i++; //Skip the second byte of the short
    return new TagShort(name,shortPayload);
}

TagInt *parseInt(const std::string &data, int &i)
{
    i++;
    int nlen = int(data[i]);
    i+=2;
    std::string name = data.substr(i,nlen);
    i+=nlen;
    int intPayload = convertToInt(data.substr(i,4));
    std::cout << "INT: " << name << " = " << intPayload << std::endl;
    i+=3; //Skip the final three bytes of the int
    return new TagInt(name,intPayload);
}

TagLong *parseLong(const std::string &data, int &i)
{
    i++;
    int nlen = int(data[i]);
    i+=2;
    std::string name = data.substr(i,nlen);
    i+=nlen;
    long longPayload = convertToLong(data.substr(i,8));
    std::cout << "LONG: " << name << " = " << longPayload << std::endl;
    i+=7; //Skip the final 7 bytes of the long
    return new TagLong(name,longPayload);
}

TagFloat *parseFloat(const std::string &data, int &i)
{
    i++;
    int nlen = int(data[i]);
    i+=2;
    std::string name = data.substr(i,nlen);
    i+=nlen;
    float floatPayload = convertToFloat(data.substr(i,4));
    std::cout << "FLOAT: " << name << " = " << floatPayload << std::endl;
    i+=3; //Skip the final 3 bytes of the float
    return new TagFloat(name,floatPayload);
}

//TODO: Double check endianness
TagDouble *parseDouble(const std::string &data, int &i)
{
    i++;
    int nlen = int(data[i]);
    i+=2;
    std::string name = data.substr(i,nlen);
    i+=nlen;
    double doublePayload = convertToDouble(data.substr(i,8));
    std::cout << "DOUBLE: " << name << " = " << doublePayload << std::endl;
    i+=7; //Skip the final 7 bytes of the double
    return new TagDouble(name,doublePayload);
}

//FIXME: Need to investigate how an NBT byte array works and redo this
TagByteArray *parseByteArray(const std::string &data, int &i)
{
    i++;
    int nlen = int(data[i]);
    i+=2;
    std::string name = data.substr(i,nlen);
    i+=nlen;
    for(int j=0; j < int(data[i]); j++){
        std::cout << data[i+j];
    }
    //i+=someamount;
    std::cout << "BYTEARRAY: " << std::endl;
    return new TagByteArray(name);
}

TagString *parseString(const std::string &data, int &i)
{
    i++;
    int nlen = int(data[i]);
    i+=2;
    std::string name = data.substr(i,nlen);
    i+=nlen;
    int payloadlen = int(data[i]);
    i++;
    std::string payload = data.substr(i,payloadlen+1);
    i+=payloadlen;
    std::cout << "STRING: " << name << " = \"" << payload << "\"" << std::endl;
    return new TagString(name,payload);
}

TagList *parseList(const std::string &data, int &i)
{
    i++;
    int nlen = int(data[i]);
    i+=2;
    std::string name = data.substr(i,nlen);
    i+=nlen;
    int tagType = int(data[i]);
    i++;
    int tagCount = convertToInt(data.substr(i,4));
    TagList *payload = new TagList(name,tagType,tagCount);
    i+=3;
    std::cout << "LIST: " << name << std::endl;
    switch (tagType)
    {
    case 1:
        for(int j=0; j < tagCount; j++){
            std::cout << "BYTE: " << data[i+j] << std::endl;
            payload->addChild(new TagByte("",data[i+j]));
        }
        i+=tagCount;
        break;
    case 2:
        for(int j=0; j < tagCount; j++){
            short shortPayload = convertToShort(data.substr(i+(j*2),2));
            std::cout << "SHORT: " << shortPayload  << std::endl;
            payload->addChild(new TagShort("",shortPayload));
        }
        i+=(tagCount*2);
        break;
    case 3:
        for(int j=0; j < tagCount; j++){
            int intPayload = convertToInt(data.substr(i+(j*4),4));
            std::cout << "INT: " << intPayload << std::endl;
            payload->addChild(new TagInt("",intPayload));
        }
        i+=(tagCount*4);
        break;
    case 4:
        for(int j=0; j < tagCount; j++){
            long longPayload = convertToLong(data.substr(i+(j*8),8));
            std::cout << "LONG: " << longPayload << std::endl;
            payload->addChild(new TagLong("",longPayload));
        }
        i+=(tagCount*8);
        break;
    case 5:
    // Not sure why we add and subtract three here, but it works
        i+=3;
        for(int j=0; j < tagCount; j++){
            float floatPayload = convertToFloatMBE(data.substr(i+(j*4),4));
            std::cout << "FLOAT: " << floatPayload << std::endl;
            payload->addChild(new TagFloat("",floatPayload));
        }
        i-=3;
        i+=(tagCount*4);
        break;
    //TODO: Check endianness and offset for parsing doubles
    case 6:
        for(int j=0; j < tagCount; j++){
            double doublePayload = convertToDouble(data.substr(i+(j*8),8));
            std::cout << "DOUBLE: " << doublePayload << std::endl;
            payload->addChild(new TagDouble("",doublePayload));
        }
        i+=(tagCount*8);
        break;
    case 7:
        //TODO: Add this later, after figuring out how this tag works
        break;
    case 8:
        {
            i++;
            for(int k=0; k<tagCount; k++)
            {
                std::cout << "STRING: ";
                int strSize = int(data[i]);
                i+=2;
                std::string stringPayload = data.substr(i,strSize);
                std::cout << stringPayload << std::endl;
                payload->addChild(new TagString("",stringPayload));
                i+=strSize;
            }
            i--;
            break;
        }

    case 10:
        {
            for(int k=0; k<tagCount; k++)
            {
                std::cout << "COMPOUND" << std::endl;
                i++;
                payload->addChild(parseUnnamedCompound(data,i));
            }
        }
    default:
        break;
    }
    return payload;
}

TagCompound *parseCompound(const std::string &data, int &i)
{
    i++;
    int nlen = int(data[i]);
    i+=2;
    std::string name = data.substr(i,nlen);
    i+=nlen;
    std::cout << "COMPOUND: " << name << std::endl;

    TagCompound *root = new TagCompound(name);

    for(i; i < data.length(); i++){

        switch (data[i])
        {
            case 0: //End
            {
                std::cout << "[END]" << std::endl;
                //root->addChild(new TagEnd());
                return root;
                break;
            }

            case 1: //Byte (or boolean, or any data that can be stored in a single byte)
                {
                    root->addChild(parseByte(data,i));
                    break;
                }

            case 2: //Short
                {
                    root->addChild(parseShort(data,i));
                    break;
                }
            
            case 3: //Int
                {
                    root->addChild(parseInt(data,i));
                    break;
                }

            case 4: //Long
                {
                    root->addChild(parseLong(data,i));
                    break;
                }

            case 5: //Float
                {
                    root->addChild(parseFloat(data,i));
                    break;
                }

            case 6: //Double
                {
                    root->addChild(parseDouble(data,i));
                    break;
                }

            case 7: //Byte Array
                {
                    root->addChild(parseByteArray(data,i));
                    break;
                }

            case 8: //String
                {
                    root->addChild(parseString(data,i));
                    break;
                }

            case 9: //List
                {
                    root->addChild(parseList(data,i));
                    break;
                }

            case 10: //Compound
                {
                    root->addChild(parseCompound(data,i));
                    break;
                }

            //TODO: Add 11 & 12 (Int Array and Long Array)

            default:
                {std::cout << "I don't know how to handle a tag of type [" << data[i] << "]!" << std::endl;
                std::cout << "(encountered at position i=" << i << ")" << std::endl;
                exit(1);
                break;}
        }
    }
}

TagCompound *parseUnnamedCompound(const std::string &data, int &i)
{
    TagCompound *root = new TagCompound();

    for(i; i < data.length(); i++){

        switch (data[i])
        {
            case 0: //End
            {
                std::cout << "[END]" << std::endl;
                //root->addChild(new TagEnd());
                return root;
                break;
            }

            case 1: //Byte (or boolean, or any data that can be stored in a single byte)
                {
                    root->addChild(parseByte(data,i));
                    break;
                }

            case 2: //Short
                {
                    root->addChild(parseShort(data,i));
                    break;
                }
            
            case 3: //Int
                {
                    root->addChild(parseInt(data,i));
                    break;
                }

            case 4: //Long
                {
                    root->addChild(parseLong(data,i));
                    break;
                }

            case 5: //Float
                {
                    root->addChild(parseFloat(data,i));
                    break;
                }

            case 6: //Double
                {
                    root->addChild(parseDouble(data,i));
                    break;
                }

            case 7: //Byte Array
                {
                    root->addChild(parseByteArray(data,i));
                    break;
                }

            case 8: //String
                {
                    root->addChild(parseString(data,i));
                    break;
                }

            case 9: //List
                {
                    root->addChild(parseList(data,i));
                    break;
                }

            case 10: //Compound
                {
                    root->addChild(parseCompound(data,i));
                    break;
                }

            //TODO: Add 11 & 12 (Int Array and Long Array)

            default:
                {std::cout << "I don't know how to handle a tag of type [" << data[i] << "]!" << std::endl;
                std::cout << "(encountered at position i=" << i << ")" << std::endl;
                exit(1);
                break;}
        }
    }
}

TagCompound parseNBT(const std::string data){

    TagCompound *root = new TagCompound();

    for(int i=3; i < data.length(); i++){

        switch (data[i])
        {
            case 0: //End
            {
                std::cout << "[END]" << std::endl;
                //root->addChild(new TagEnd());
                return *root;
                break;
            }

            case 1: //Byte (or boolean, or any data that can be stored in a single byte)
                {
                    root->addChild(parseByte(data,i));
                    break;
                }

            case 2: //Short
                {
                    root->addChild(parseShort(data,i));
                    break;
                }
            
            case 3: //Int
                {
                    root->addChild(parseInt(data,i));
                    break;
                }

            case 4: //Long
                {
                    root->addChild(parseLong(data,i));
                    break;
                }

            case 5: //Float
                {
                    root->addChild(parseFloat(data,i));
                    break;
                }

            case 6: //Double
                {
                    root->addChild(parseDouble(data,i));
                    break;
                }

            case 7: //Byte Array
                {
                    root->addChild(parseByteArray(data,i));
                    break;
                }

            case 8: //String
                {
                    root->addChild(parseString(data,i));
                    break;
                }

            case 9: //List
                {
                    root->addChild(parseList(data,i));
                    break;
                }

            case 10: //Compound
                {
                    root->addChild(parseCompound(data,i));
                    break;
                }

            //TODO: Add 11 & 12 (Int Array and Long Array)

            default:
                {std::cout << "I don't know how to handle a tag of type [" << data[i] << "]!" << std::endl;
                std::cout << "(encountered at position i=" << i << ")" << std::endl;
                exit(1);
                break;}
        }
    }

    return *root;
}