/**
 * @file Attributes.h
 * @author Chenxi Zhou
 * @brief The wapper of json like key-value set
 * @version 0.1
 * @date 2022-07-04
 * 
 * @copyright NJUMeta (c) 2022
 * www.njumeta.com
 */
#pragma once
#include "rapidjson/document.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>

struct Value {
    Value() = default;
    virtual ~Value() = default;

    virtual void print() const = 0;
}; 

class Attribute {
public:
    Attribute() : m_type(EValueType::EInvalid), m_value(nullptr) { }

    Attribute(const rapidjson::Document &document);
    
    Attribute(const std::string &key, bool b);
    
    Attribute(const std::string &key, int i);

    Attribute(const std::string &key, double d);

    Attribute(const std::string &key, const std::string &str);

    Attribute(const std::string &key,
              const rapidjson::GenericValue<rapidjson::UTF8<>>::ConstArray &array);

    Attribute(const std::string &key,
              const rapidjson::GenericValue<rapidjson::UTF8<>>::ConstObject &object);


    void print() const {
        std::cout << "Key = " << m_key << ", ";
        m_value->print();
    }

    Attribute operator[](const std::string &key) const;

    Attribute operator[](int i) const;

    bool getBool() const;
    
    int getInt() const;

    double getDouble() const;

    std::string getString() const;

private:

    enum class EValueType {
        EInvalid = 0,
        EBool,
        EInt,
        EDouble,
        EString,
        EArray,
        EObject
    } m_type;

    std::string m_key;

    std::shared_ptr<Value> m_value;
    
};


struct BoolValue : public Value {
    BoolValue() = default;

    BoolValue(bool b) : data(b) { };

    virtual ~BoolValue() = default;

    virtual void print() const override {
        std::cout << "BoolValue : " << (data ? "true" : "false") << std::endl;    
    }
    
    bool data;

};

struct IntValue : public Value {
    IntValue() = default;

    IntValue(int i) : data(i) { }

    virtual ~IntValue() = default;

    int data; 

    virtual void print() const override {
        std::cout << "IntValue : " << data << std::endl;    
    }   
};


struct DoubleValue : public Value {
    DoubleValue() = default;

    DoubleValue(double d) : data(d) { }

    virtual ~DoubleValue() = default;

    double data;

    virtual void print() const override {
        std::cout << "DoubleValue : " << data << std::endl;
    }
};


struct ArrayValue : public Value {
    ArrayValue() = default;

    virtual ~ArrayValue() {
        for (int i = 0; i < data.size(); ++i) {
            delete data[i];
        }
    }

    std::vector<Attribute *> data;

    virtual void print() const override {
        std::cout << "ArrayValue : \n";
        for (int i = 0; i < data.size(); ++i) {
            std::cout << "\t";
            data[i]->print();
        }
    }
};

struct StringValue : public Value {
    StringValue() = default;

    StringValue(const std::string &str) : data(str) { }

    virtual ~StringValue() = default;

    virtual void print() const override {
        std::cout << "StringValue : " << data << std::endl;
    }

    std::string data;

};

struct ObjectValue : public Value {
    ObjectValue() = default;

    virtual ~ObjectValue() {
        for (auto itr = data.begin(); itr != data.end(); ++itr) {
            delete itr->second;
        }
    }

    virtual void print() const override {
        std::cout << "ObjectValue : {\n";
        for (auto itr = data.begin(); itr!=data.end(); ++itr) {
            std::cout << "\t";
            itr->second->print();
        }
        std::cout <<"}"<<std::endl;
    }

    std::unordered_map<std::string, Attribute *> data;
};


inline Attribute::Attribute(const rapidjson::Document &document) {
    m_type = EValueType::EObject;
    m_key = "root";
    m_value = std::make_shared<ObjectValue>();

    ObjectValue* object_value = static_cast<ObjectValue *>(m_value.get());

    for (auto itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
        Attribute *attribute = nullptr;
        const std::string &member_key = itr->name.GetString();
        const auto &member_value = itr->value;
        if (member_value.IsBool()) {
            attribute = new Attribute(member_key, member_value.GetBool());
        } else if (member_value.IsInt()) {
            attribute = new Attribute(member_key, member_value.GetInt());
        } else if (member_value.IsDouble()) {
            attribute = new Attribute(member_key, member_value.GetDouble());
        } else if (member_value.IsString()) {
            attribute = new Attribute(member_key, std::string(member_value.GetString()));
        } else if (member_value.IsArray()) {
            attribute = new Attribute(member_key, member_value.GetArray());
        } else if (member_value.IsObject()) {
            attribute = new Attribute(member_key, member_value.GetObject());
        } 
        if (attribute == nullptr){
            std::cout << "Error type\n";
            std::exit(1);
        }
        object_value->data[member_key] = attribute;
    }
}

inline Attribute::Attribute(const std::string &key, bool b) :
    m_type(EValueType::EBool),
    m_key(key),
    m_value(new BoolValue {b}) { }


inline Attribute::Attribute(const std::string &key, int i) : 
    m_type(EValueType::EInt),
    m_key(key), 
    m_value(new IntValue {i}) { }

inline Attribute::Attribute(const std::string &key, double d) : 
    m_type(EValueType::EDouble),
    m_key(key),
    m_value(new DoubleValue {d}) { }

inline Attribute::Attribute(const std::string &key, const std::string &str) :
    m_type(EValueType::EString),
    m_key(key),
    m_value(new StringValue {str}) { }


inline Attribute::Attribute(
    const std::string &key,
    const rapidjson::GenericValue<rapidjson::UTF8<>>::ConstArray &array) :
    m_type(EValueType::EArray),
    m_key(key) {
    
    m_value = std::make_shared<ArrayValue>();
    ArrayValue *array_value = static_cast<ArrayValue *>(m_value.get());
    for (auto itr = array.Begin(); itr != array.End(); ++itr) {
        Attribute *attribute = nullptr;
        if (itr->IsBool()) {
            attribute = new Attribute("", itr->GetBool());
        } else if (itr->IsInt()) {
            attribute = new Attribute("", itr->GetInt());
        } else if (itr->IsDouble()) {
            attribute = new Attribute("", itr->GetDouble());
        } else if (itr->IsString()) {
            attribute = new Attribute("", std::string(itr->GetString()));
        } else if (itr->IsArray()) {
            attribute = new Attribute("", itr->GetArray());
        } else if (itr->IsObject()) {
            attribute = new Attribute("", itr->GetObject());
        }
        if (attribute == nullptr) {
            std::cout << "Error Type\n";
            std::exit(1);
        }
        array_value->data.emplace_back(attribute);
    } 
}

inline Attribute::Attribute(
    const std::string &key,
    const rapidjson::GenericValue<rapidjson::UTF8<>>::ConstObject &object) :
    m_type(EValueType::EObject),
    m_key(key) {

    m_value = std::make_shared<ObjectValue>();
    ObjectValue *object_value = static_cast<ObjectValue *>(m_value.get());
    for (auto itr = object.MemberBegin(); itr != object.MemberEnd(); ++itr) {
        Attribute *attribute = nullptr;
        const std::string &member_key = itr->name.GetString();
        const auto &member_value = itr->value;
        if (member_value.IsBool()) {
            attribute = new Attribute(member_key, member_value.GetBool());
        } else if (member_value.IsInt()) {
            attribute = new Attribute(member_key, member_value.GetInt());
        } else if (member_value.IsDouble()) {
            attribute = new Attribute(member_key, member_value.GetDouble());
        } else if (member_value.IsString()) {
            attribute = new Attribute(member_key, std::string(member_value.GetString()));
        } else if (member_value.IsArray()) {
            attribute = new Attribute(member_key, member_value.GetArray());
        } else if (member_value.IsObject()) {
            attribute = new Attribute(member_key, member_value.GetObject());
        } 
        if (attribute == nullptr){
            std::cout << "Error type\n";
            std::exit(1);
        }
        object_value->data[member_key] = attribute;
    } 
}

inline Attribute Attribute::operator[](const std::string &key) const {
    assert(m_type == EValueType::EObject);
    ObjectValue *object_value = static_cast<ObjectValue *>(m_value.get());
    const auto attributes = object_value->data;
    if (auto itr = attributes.find(key); itr == attributes.end()) {
        std::cout << "No such a key \"" << key <<  "\"\n";
        std::exit(1);
    } else {
        return *(itr->second);
    }
}

inline Attribute Attribute::operator[](int i) const {
    assert(m_type == EValueType::EArray);
    ArrayValue *array_value = static_cast<ArrayValue *>(m_value.get());
    const auto attribute_list = array_value->data;
    assert(i < attribute_list.size());
    return *attribute_list[i];
}

inline bool Attribute::getBool() const {
    assert(m_type == EValueType::EBool);
    BoolValue *bool_value = static_cast<BoolValue *>(m_value.get());
    return bool_value->data;
}

inline int Attribute::getInt() const {
    assert(m_type == EValueType::EInt);
    IntValue *int_value = static_cast<IntValue *>(m_value.get());
    return int_value->data;
}

inline double Attribute::getDouble() const {
    assert(m_type == EValueType::EDouble);
    DoubleValue *double_value = static_cast<DoubleValue *>(m_value.get());
    return double_value->data;
}

inline std::string Attribute::getString() const {
    assert(m_type == EValueType::EString);
    StringValue *string_value = static_cast<StringValue *>(m_value.get());
    return string_value->data;
}

