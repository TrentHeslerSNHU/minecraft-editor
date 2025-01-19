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
    public:
        TagList(){this->TagType = new short(9); this->Name = new std::string("TagList");}
        TagList(const std::string, unsigned char, int);
        void addChild(NbtTag *);
        void listChildren();
        int numChildren();
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

//======================= The "Main Event" =======================
TagCompound getNBT(const std::string);
void parseNBT(const std::string);
#endif