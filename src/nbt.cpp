#include "nbt.h"

TagByte *getByte(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    unsigned char Payload = Data[i];
    TagByte *Tag = new TagByte(Name,Payload);
    return Tag;
}

TagShort *getShort(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    TagShort *Tag = new TagShort(Name,convertToShort(Data.substr(i,2)));
    i++;
    return Tag;
}

TagInt *getInt(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    TagInt *Tag = new TagInt(Name,convertToInt(Data.substr(i,4)));
    i+=3;
    return Tag;
}

TagLong *getLong(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    TagLong *Tag = new TagLong(Name,convertToLong(Data.substr(i,8)));
    i+=7;
    return Tag;
}

TagFloat *getFloat(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    TagFloat *Tag = new TagFloat(Name,convertToFloat(Data.substr(i,4)));
    i+=3;
    return Tag;
}

TagDouble *getDouble(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
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
    TagString *Tag = new TagString(Name,Data.substr(i,PayloadLen));
    return Tag;
}

TagList *getList(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    int itemType = int(Data[i]);
    int itemQty = int(Data[i+1]);
    i+=5;

    TagList *Tag = new TagList(Name,itemType,itemQty);
    
    switch (itemType)
    {
    case ByteTag:
    {
        for (size_t j = 0; j < itemQty; j++)
        {
            Tag->addListItem(Data[i+j]);
        }
        i+=itemQty;
        break;
    }

    case ShortTag:
    {
        for (size_t j = 0; j < itemQty; j++)
        {
            Tag->addListItem(convertToShort(Data.substr(i+(j*2),2)));
        }
        i+=(itemQty*2);
        break;
    }
    
    case IntTag:
    {
        for (size_t j = 0; j < itemQty; j++)
        {
            Tag->addListItem(convertToInt(Data.substr(i+(j*4),4)));
        }
        i+=(itemQty*4);
        break;
    }

    case LongTag:
    {
        for (size_t j = 0; j < itemQty; j++)
        {
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
            Tag->addListItem(Data.substr(i,strSize));
            i+=strSize;
        }
        i--;
        break;
    }

    default:
        break;
    }

    return Tag;
}

TagCompound *getCompound(const std::string &Data, int &i){
    short nlen = short(Data[i]);
    i+=2;
    std::string Name = Data.substr(i,nlen);
    i+=nlen;
    TagCompound *Tag;
    return Tag;
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
            root->appendChild(new TagEnd());
            break;
        }
        
        case ByteTag://1
        {
            root->appendChild(getByte(Data,i));
            break;
        }

        case ShortTag://2
        {
            root->appendChild(getShort(Data,i));
            break;
        }

        case IntTag://3
        {
            root->appendChild(getInt(Data,i));
            break;
        }

        case LongTag://4
        {
            root->appendChild(getLong(Data,i));
            break;
        }

        case FloatTag://5
        {
            root->appendChild(getFloat(Data,i));
            break;
        }

        case DoubleTag://6
        {
            root->appendChild(getDouble(Data,i));
            break;
        }

        case ByteArrayTag://7
        {
            root->appendChild(getByteArray(Data,i));
            break;
        }

        case StringTag://8
        {
            root->appendChild(getString(Data,i));
            break;
        }

        case ListTag://9
        {
            root->appendChild(getList(Data,i));
            break;
        }

        case CompoundTag://10
        {
            root->appendChild(getCompound(Data,i));
            break;
        }

        default:
        {
            break;
        }
       }
    }
    
    return *root;
}