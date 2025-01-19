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
                    std::cout << "(" << lstPtr->numChildren() << ")\n";
                    lstPtr->listChildren();
                } else if (nextTag->getType() == 10) {
                    TagCompound *cmpPtr = static_cast<TagCompound *>(nextTag);
                    std::cout << "(" << cmpPtr->numChildren() << ")\n";
                    cmpPtr->listChildren();
                } else {
                    std::cout << std::endl;
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
                std::cout << "LIST: " << currentItem->getName() << std::endl;
                break;
            }

            case 10:
            {
                std::cout << "COMPOUND: " << currentItem->getName() << "(" << ((TagCompound *)currentItem)->numChildren() << ")" << std::endl;
                reinterpret_cast<TagCompound *>(currentItem)->listChildren();
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
    std::string name = Data.substr(i,nlen);
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
    std::string name = Data.substr(i,nlen);
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
    std::string name = Data.substr(i,nlen);
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
    std::string name = Data.substr(i,nlen);
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

//FIXME: Redo this once you know what a bytearray looks like
TagByteArray *getByteArray(const std::string &Data, int &i) {
    if(PRINT_TAG_BOUNDS){
        std::cout << "<" << i << ">\n";
    }
    i++;
    int nlen = Data[i];
    i+=2;
    std::string name = Data.substr(i,nlen);
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
    std::string name = Data.substr(i,nlen);
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
    std::string name = Data.substr(i,nlen);
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
    std::string name = Data.substr(i,nlen);
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

void parseNBT(const std::string data){

    TagCompound *root = new TagCompound();

    for(int i=3; i < data.length(); i++){
        std::string name = "";
        int nlen = 0;
        short shortPayload = 0;
        int intPayload = 0;
        long longPayload = 0;
        float floatPayload = 0;
        double doublePayload = 0;
        std::string stringPayload = "";

        switch (data[i])
        {
            case 0: //End
            {
                std::cout << "[END]" << std::endl;
                break;
            }

            case 1: //Byte (or boolean, or any data that can be stored in a single byte)
                {i++;
                nlen = int(data[i]);
                name = "";
                i++;
                for(int j=0; j <= nlen; j++){
                    name += data[i+j];
                }
                i+=nlen+1;
                unsigned char payload = (unsigned char)data[i];
                std::cout << "BYTE: " << name << " = " << payload/1 << std::endl;
                root->addChild(new TagByte(name,payload));
                break;}

            case 2: //Short
                {i++;
                nlen = int(data[i]);
                name = "";
                i++;
                for(int j=0; j <= nlen; j++){
                    name += data[i+j];
                }
                i+=nlen+1;
                shortPayload=convertToShort(data.substr(i,2));
                std::cout << "SHORT: " << name << " = " << shortPayload << std::endl;
                root->addChild(new TagShort(name,shortPayload));
                i++; //Skip the second byte of the short
                break;}
            
            case 3: //Int
                {i++;
                nlen = int(data[i]);
                name = "";
                i++;
                for(int j=0; j <= nlen; j++){
                    name += data[i+j];
                }
                i+=nlen+1;
                intPayload = convertToInt(data.substr(i,4));
                std::cout << "INT: " << name << " = " << intPayload << std::endl;
                root->addChild(new TagInt(name,intPayload));
                i+=3; //Skip the final three bytes of the int
                break;}

            case 4: //Long
                {i++;
                nlen = int(data[i]);
                name = "";
                i++;
                for(int j=0; j <= nlen; j++){
                    name += data[i+j];
                }
                i+=nlen+1;
                longPayload = convertToLong(data.substr(i,8));
                std::cout << "LONG: " << name << " = " << longPayload << std::endl;
                root->addChild(new TagLong(name,longPayload));
                i+=7; //Skip the final 7 bytes of the long
                break;}

            case 5: //Float
                {i++;
                nlen = int(data[i]);
                name = "";
                i++;
                for(int j=0; j <= nlen; j++){
                    name += data[i+j];
                }
                i+=nlen+1;
                floatPayload = convertToFloat(data.substr(i,4));
                std::cout << "FLOAT: " << name << " = " << floatPayload << std::endl;
                root->addChild(new TagFloat(name,floatPayload));
                i+=3; //Skip the final 3 bytes of the float
                break;}

            case 6: //Double
                {i++;
                nlen = int(data[i]);
                name = "";
                i++;
                for(int j=0; j <= nlen; j++){
                    name += data[i+j];
                }
                i+=nlen+1;
                doublePayload = convertToDouble(data.substr(i,8));
                std::cout << "DOUBLE: " << name << " = " << doublePayload << std::endl;
                root->addChild(new TagDouble(name,doublePayload));
                i+=7; //Skip the final 7 bytes of the double
                break;}

            case 7: //Byte Array
            //FIXME: Need to investigate how an NBT byte array works and redo this
                {i++;
                nlen = int(data[i]);
                name = "";
                i++;
                for(int j=0; j <= nlen; j++){
                    name += data[i+j];
                }
                i+=nlen+1;
                for(int j=0; j < int(data[i]); j++){
                    std::cout << data[i+j];
                }
                //i+=someamount;
                std::cout << "BYTEARRAY: " << std::endl;
                root->addChild(new TagByteArray(name));
                break;}

            case 8: //String
                {i++;
                nlen = int(data[i]);
                name = "";
                i++;
                for(int j=0; j <= nlen; j++){
                    name += data[i+j];
                }
                i+=nlen+1;
                std::string payload = "";
                int payloadlen = int(data[i]);
                i++;
                for(int j=0; j <= payloadlen; j++){
                    payload += data[i+j];
                }
                i+=payloadlen;
                std::cout << "STRING: " << name << " = \"" << payload << "\"" << std::endl;
                root->addChild(new TagString(name,payload));
                break;}

            case 9: //List
                {i++;
                nlen = int(data[i]);
                name = "";
                i++;
                for(int j=0; j <= nlen; j++){
                    name += data[i+j];
                }
                i+=nlen;
                i++;
                int tagType = int(data[i]);
                i++;
                int tagCount = 0;
                unsigned char bytes[] = {data[i], data[i+1], data[i+2], data[i+3]};
                memcpy(&tagCount, &bytes, sizeof(int));
                root->addChild(new TagList(name,tagType,tagCount));
                i+=3;
                std::cout << "LIST: " << name << std::endl;
                switch (tagType)
                {
                case 1:
                    for(int j=0; j < tagCount; j++){
                        std::cout << "BYTE: " << data[i+j] << std::endl;
                    }
                    i+=tagCount;
                    break;
                case 2:
                    //TODO: Add loop
                    for(int j=0; j < tagCount; j++){
                        std::cout << "SHORT: " << convertToShort(data.substr(i+(j*2),2)) << std::endl;
                    }
                    i+=(tagCount*2);
                    break;
                case 3:
                    //TODO: Add loop
                    for(int j=0; j < tagCount; j++){
                        std::cout << "INT: " << convertToInt(data.substr(i+(j*4),4)) << std::endl;
                    }
                    i+=(tagCount*4);
                    break;
                case 4:
                    //TODO: Add loop
                    for(int j=0; j < tagCount; j++){
                        std::cout << "LONG: " << convertToLong(data.substr(i+(j*8),8)) << std::endl;
                    }
                    i+=(tagCount*8);
                    break;
                case 5:
                    //TODO: Remove add/subtract 3 (qty should be 32-bit int?)
                    i+=3;
                    for(int j=0; j < tagCount; j++){
                        std::cout << "FLOAT: " << convertToFloatMBE(data.substr(i+(j*4),4)) << std::endl;
                    }
                    i-=3;
                    i+=(tagCount*4);
                    break;
                case 6:
                    //TODO: Add loop
                    for(int j=0; j < tagCount; j++){
                        std::cout << "DOUBLE: " << convertToDouble(data.substr(i+(j*8),8)) << std::endl;
                    }
                    i+=(tagCount*8);
                    break;
                case 7:
                    //TODO: Add loop
                    break;
                case 8:
                    {
                        i++;
                        for(int k=0; k<tagCount; k++)
                        {
                            std::cout << "STRING: ";
                            int strSize = int(data[i]);
                            i+=2;
                            for(int l=0; l<strSize; l++){
                                std::cout << data[i+l];
                            }
                            std::cout << std::endl;
                            i+=strSize;
                        }
                        i--;
                        break;
                    }
                default:
                    break;
                }
                break;}

            case 10: //Compound
            {
                i++;
                nlen = int(data[i]);
                name = "";
                i++;
                for(int j=0; j <= nlen; j++){
                    name += data[i+j];
                }
                i+=nlen;
                std::cout << "COMPOUND: " << name << std::endl;
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