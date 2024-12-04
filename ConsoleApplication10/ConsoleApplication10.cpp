//IPrint.h
#ifndef IPRINT_H
#define IPRINT_H

class IPrint {
public:
    virtual ~IPrint() = default;
    virtual void print() const = 0;
};

#endif
//Array.h
#ifndef ARRAY_H
#define ARRAY_H

#include "IPrint.h"
#include <iostream>
#include <stdexcept>

template <typename T>
class Array : public IPrint {
private:
    T* data;
    size_t size;
    size_t capacity;

    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    Array(size_t initialCapacity = 10)
        : size(0), capacity(initialCapacity), data(new T[initialCapacity]) {}

    ~Array() {
        delete[] data;
    }

    void add(const T& item) {
        if (size == capacity) {
            resize();
        }
        data[size++] = item;
    }

    void remove(size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        for (size_t i = index; i < size - 1; i++) {
            data[i] = data[i + 1];
        }
        size--;
    }

    T& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    size_t getSize() const {
        return size;
    }

    void print() const override {
        for (size_t i = 0; i < size; i++) {
            data[i]->print();
        }
    }
};

#endif
//Unit.h
#ifndef UNIT_H
#define UNIT_H

#include "IPrint.h"
#include "Position.h"
#include <string>

class Unit : public IPrint {
protected:
    std::string name;
    int health;
    Position position;

public:
    Unit(const std::string& name, int health, int x, int y);
    virtual ~Unit() = default;

    std::string getName() const;
    int getHealth() const;
    Position getPosition() const;

    void setName(const std::string& name);
    void setHealth(int health);
    void setPosition(int x, int y);

    void move(int dx, int dy);
    void moveTowards(const Unit& target);
    int distanceTo(const Unit& other) const;

    virtual void print() const override = 0;
};

#endif
//Unit.cpp
#include "Unit.h"
#include <iostream>

Unit::Unit(const std::string& name, int health, int x, int y)
    : name(name), health(health), position(x, y) {}

std::string Unit::getName() const {
    return name;
}

int Unit::getHealth() const {
    return health;
}

Position Unit::getPosition() const {
    return position;
}

void Unit::setName(const std::string& name) {
    this->name = name;
}

void Unit::setHealth(int health) {
    this->health = health;
}

void Unit::setPosition(int x, int y) {
    position.setX(x);
    position.setY(y);
}

void Unit::move(int dx, int dy) {
    position.setX(position.getX() + dx);
    position.setY(position.getY() + dy);
}

void Unit::moveTowards(const Unit& target) {
    position.moveTowards(target.getPosition());
}

int Unit::distanceTo(const Unit& other) const {
    return position.distance(other.getPosition());
}
//Team.h
#ifndef TEAM_H
#define TEAM_H

#include "Unit.h"
#include "Array.h"
#include <string>

class Team : public IPrint {
private:
    std::string teamName;
    Array<Unit*> members;

public:
    Team(const std::string& teamName);

    void addMember(Unit* unit);
    void removeMember(const std::string& name);
    Unit* findMember(const std::string& name);

    void print() const override;
};

#endif
//Team.cpp
#include "Team.h"
#include <iostream>

Team::Team(const std::string& teamName) : teamName(teamName) {}

void Team::addMember(Unit* unit) {
    members.add(unit);
}

void Team::removeMember(const std::string& name) {
    for (size_t i = 0; i < members.getSize(); i++) {
        if (members[i]->getName() == name) {
            members.remove(i);
            return;
        }
    }
}

Unit* Team::findMember(const std::string& name) {
    for (size_t i = 0; i < members.getSize(); i++) {
        if (members[i]->getName() == name) {
            return members[i];
        }
    }
    return nullptr;
}

void Team::print() const {
    std::cout << "Team: " << teamName << "\n";
    members.print();
}
//DerivedUnit.h
#ifndef DERIVED_UNIT_H
#define DERIVED_UNIT_H

#include "Unit.h"
#include <iostream>

class Warrior : public Unit {
public:
    Warrior(const std::string& name, int health, int x, int y)
        : Unit(name, health, x, y) {}

    void print() const override {
        std::cout << "Warrior: " << name << ", Health: " << health
            << ", Position: (" << position.getX() << ", " << position.getY() << ")\n";
    }
};

#endif
//main.cpp
#include "Team.h"
#include "DerivedUnit.h"
#include <iostream>

int main() {
    Warrior* w1 = new Warrior("Aragorn", 100, 0, 0);
    Warrior* w2 = new Warrior("Boromir", 90, 1, 1);

    Team team("Fellowship");
    team.addMember(w1);
    team.addMember(w2);

    team.print();

    delete w1;
    delete w2;

    return 0;
}
