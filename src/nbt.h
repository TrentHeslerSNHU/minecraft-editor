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

//TODO: Add getPayload functions! :)

//Abstract class
class NbtTag {
    public:
        short getType(){return *(this->TagType);}
        NbtTag* getNext(){return this->Next;}
        void setNext(NbtTag *newNext){this->Next = newNext;}
        std::string getName(){return *(this->Name);}
        virtual const std::string getPayload() = 0;
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
        const std::string getPayload() {return "";};
};

//TYPE 1: Byte
class TagByte : public NbtTag {
    public:
        TagByte(){this->TagType = new short(1); this->Name=new std::string("TagByte");}
        TagByte(const std::string, unsigned char);
        const std::string getPayload() {return std::to_string(*Payload);};
    private:
        unsigned char *Payload = nullptr;
};

//TYPE 2: Short
class TagShort : public NbtTag {
    public:
        TagShort(){this->TagType = new short(2); this->Name=new std::string("TagShort");}
        TagShort(const std::string, short);
        const std::string getPayload() {return std::to_string(*Payload);};
    private:
        short *Payload = nullptr;
};

//TYPE 3: Int
class TagInt : public NbtTag {
    public:
        TagInt(){this->TagType = new short(3); this->Name=new std::string("TagInt");}
        TagInt(const std::string, int);
        const std::string getPayload() {return std::to_string(*Payload);};
    private:
        int *Payload = nullptr;
};

//TYPE 4: Long
class TagLong : public NbtTag {
    public:
        TagLong(){this->TagType = new short(4); this->Name=new std::string("TagLong");}
        TagLong(const std::string, long);
        const std::string getPayload() {return std::to_string(*Payload);};
    private:
        long *Payload = nullptr;
};

//TYPE 5: Float
class TagFloat : public NbtTag {
    public:
        TagFloat(){this->TagType = new short(5); this->Name=new std::string("TagFloat");}
        TagFloat(const std::string, float);
        const std::string getPayload() {return std::to_string(*Payload);};
    private:
        float *Payload = nullptr;
};

//TYPE 6: Double
class TagDouble : public NbtTag {
    public:
        TagDouble(){this->TagType = new short(6); this->Name=new std::string("TagDouble");}
        TagDouble(const std::string, double);
        const std::string getPayload() {return std::to_string(*Payload);};
    private:
        double *Payload = nullptr;
};

//TYPE 7: Byte Array
class TagByteArray : public NbtTag {
    public:
        TagByteArray(){this->TagType = new short(7); this->Name=new std::string("TagByteArray");}
        TagByteArray(const std::string);
        const std::string getPayload() {return std::to_string(*Payload);};
    private:
        int *Length = 0;
        unsigned char *Payload = nullptr;
};

//TYPE 8: String
class TagString : public NbtTag {
    public:
        TagString(){this->TagType = new short(8); this->Name=new std::string("TagString");}
        TagString(const std::string, const std::string);
        const std::string getPayload(){return *Payload;}
    private:
        std::string *Payload = nullptr;
};

//TYPE 9: List
class TagList : public NbtTag {
    public:
        TagList(){this->TagType = new short(9); this->Name = new std::string("TagList");}
        TagList(const std::string, unsigned char, int);
        void addChild(NbtTag *);
        void listChildren();
        int numChildren();
        const std::string getPayload() {return "";};
    private:
        unsigned char *itemType;
        int *itemQty;
        NbtTag *Children = nullptr;
};

//TYPE 10: Compound
class TagCompound : public NbtTag {
    public:
        TagCompound(){this->TagType = new short(10); this->Name = new std::string("TagCompound");}
        TagCompound(const std::string);
        void addChild(NbtTag *);
        void listChildren();
        int numChildren();
        const std::string getPayload() {return "";};
    private:
        NbtTag *Parent = nullptr;
        NbtTag *Children = nullptr;
};

//TODO: Add types 11 and 12 (IntArray and LongArray)

//======================= Helper Functions =======================

short convertToShort(const std::string);
int convertToInt(const std::string);
long convertToLong(const std::string);
float convertToFloat(const std::string);
float convertToFloatMBE(const std::string);
double convertToDouble(const std::string);

//=======================   TagBuilders   =======================
TagByte *getByte(const std::string &, int &);
TagShort *getShort(const std::string &, int &);
TagInt *getInt(const std::string &, int &i);
TagLong *getLong(const std::string &, int &);
TagFloat *getFloat(const std::string &, int &);
TagDouble *getDouble(const std::string &, int &);
TagByteArray *getByteArray(const std::string &, int &);
TagString *getString(const std::string &, int &);
TagList *getList(const std::string &, int &);
TagCompound *getCompound(const std::string &, int &);
TagCompound *getUnnamedCompound(const std::string &, int &);
TagEnd *getEnd(int &);

TagByte *parseByte(const std::string &, int &);
TagShort *parseShort(const std::string &, int &);
TagInt *parseInt(const std::string &, int &);
TagLong *parseLong(const std::string &, int &);
TagFloat *parseFloat(const std::string &, int &);
TagDouble *parseDouble(const std::string &, int &);
TagByteArray *parseByteArray(const std::string &, int &);
TagString *parseString(const std::string &, int &);
TagList *parseList(const std::string &, int &);
TagCompound *parseCompound(const std::string &, int &);
TagCompound *parseUnnamedCompound(const std::string &, int &);

//======================= The "Main Event" =======================
TagCompound getNBT(const std::string);
TagCompound parseNBT(const std::string);
#endif