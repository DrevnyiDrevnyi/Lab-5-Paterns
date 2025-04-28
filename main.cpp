#include <iostream>
#include <cstdint>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Fill.h"

using namespace std;
enum class UnitFaction: int
{
    Orden,
    Necropolis,
    Academy,
    Neutral
};

class Units
{
private:
    UnitFaction Faction;
    int Tier;
protected:
    int Heal;
    int Sell;
    double Dmg;
public:
    Units (UnitFaction faction): Faction(faction), Tier(1), Heal(1), Sell(1), Dmg(1.0)
    {
        cout<< "Unit from your fraction is ready" << endl;
    }
    virtual ~Units(){}

    virtual int GetHeal() {return Heal;}
    virtual int GetSell() {return Sell;}
    virtual double GetDmg() {return Dmg;}
    UnitFaction GetFaction() const {return Faction;}

};

class Gremlin : public Units
{
private:
    int Proj;
public:
    Gremlin();
    ~Gremlin();
    int GetHeal();
    double GetDmg();
    int GetSell();
    int GetProj();
};
Gremlin ::Gremlin() : Units (UnitFaction ::Academy)
{
    Dmg = rand()%2+1;
    Heal =rand()%5+1;
    cout <<"Gremlin is buy" << endl;
}
Gremlin ::~Gremlin()
{
    cout<< "Gremlin die" <<endl;
}

int Gremlin :: GetHeal()
{
    return Heal;
}
double Gremlin :: GetDmg()
{
    cout<<"Gremlin Dmg:"<<Dmg<<endl;
    return Dmg;
}

int Gremlin :: GetProj()
{
    Proj = 5;
    cout<<"Gremlin have projectile: " << Proj << endl;
}
int Gremlin :: GetSell()
{
    Sell = 22;
    cout<<"Gremlin sell for: " << Sell<<" and " <<"Gremlin have HP: " << Heal <<endl;
}
class Peasant : public Units
{
public:
    Peasant();
    ~Peasant();
    double GetDmg();
    int GetHeal();
    int GetSell();
};
Peasant ::Peasant() : Units (UnitFaction ::Orden)
{
    Dmg = 1.0;
    Heal = rand()%3+1;
    cout <<"Peasant is buy" << endl;
}
Peasant ::~Peasant()
{
    cout<< "Peasant die" <<endl;
}

int Peasant :: GetSell()
{
    Sell = 15;
    cout<<"Peasant sell for: " << Sell << " and " << "Peasant have HP: " << Heal <<endl;
}

int Peasant :: GetHeal()
{
    return Heal;
}
double Peasant :: GetDmg()
{
    cout<<"Peasant Dmg:"<<Dmg<<endl;
    return Dmg;
}

class Skeleton : public Units
{
public:
    Skeleton();
    ~Skeleton();
    int GetHeal();
    int GetSell();
    double GetDmg();
};
Skeleton ::Skeleton() : Units (UnitFaction :: Necropolis)
{
    Dmg = 1.0;
    Heal = rand()%4+1;
    cout <<"Peasant became a Skeleton" << endl;
}
Skeleton ::~Skeleton()
{
    cout<< "Skeleton die... again" <<endl;
}

int Skeleton :: GetSell()
{
    Sell = 19;
    cout<<"Skeleton sell for: " << Sell << " and " <<"Skeleton have HP: " << Heal <<endl;
}

int Skeleton :: GetHeal()
{
    return Heal;
}
double Skeleton :: GetDmg()
{
    cout<<"Skeleton Dmg:"<<Dmg<<endl;
    return Dmg;
}

enum class UnitType : int
{
    Gremlin = 1,
    Peasant = 2,
    Skeleton = 3,
    NoUnit = 0
};
Units *CreateUnit (UnitType type)
{
    Units *newUnit =nullptr;
    if (type == UnitType ::Gremlin)
    {
        newUnit = new Gremlin;
    }
    else if (type == UnitType ::Peasant)
    {
        newUnit = new Peasant;
    }
    else if (type == UnitType ::Skeleton)
    {
        newUnit = new Skeleton;
    }
    return newUnit;
}

//Декоратор для определения фракции
class UnitFactionDecorator : public IteratorDecorator<class Units*>
{
private:
    UnitFaction TargetFaction;

public:
    UnitFactionDecorator(Iterator<Units*> *it, UnitFaction faction)
    : IteratorDecorator<Units*>(it), TargetFaction(faction) {}
    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetFaction() != TargetFaction)
        {
            It->Next();
        }
    }
    void Next()
    {
        do
        {
            It->Next();
        } while(!It->IsDone() && It->GetCurrent()->GetFaction() != TargetFaction);
    }
};

//Декоратор для отсеивания самых хилых
class UnitHealDecorator : public IteratorDecorator<class Units*>
{
private:
    int TargetHeal ;

public:
    UnitHealDecorator(Iterator<Units*> *it, int Hp)
    : IteratorDecorator<Units*>(it), TargetHeal (Hp) {}
    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetHeal() < TargetHeal)
        {
            It->Next();
        }
    }
    void Next()
    {
        do
        {
            It->Next();
        } while(!It->IsDone() && It->GetCurrent()->GetHeal() < TargetHeal);
    }
};
//Декоратор для получения информации об уроне
class UnitDmgDecorator : public IteratorDecorator<Units*>
{
public:
    UnitDmgDecorator(Iterator<Units*> *it)
    : IteratorDecorator<Units*>(it) {}

    void Next()
    {
        It->GetCurrent()->GetDmg();
        It->Next();
        wcout << endl;
    }
};





void AllUnit(Iterator<Units*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Units *currentUnit = it->GetCurrent();
        currentUnit->GetSell();
    }
}




int main()
{
    setlocale(LC_ALL, "Russian");
    srand(time(0));//без этого рандома нет

    ArrayClass<Units*> UnitArray;
    for(size_t i=0; i<10; i++)
    {
        int unit_num = rand()%3+1;
        UnitType unit_type = static_cast<UnitType>(unit_num);
        Units *newUnit = CreateUnit(unit_type);
        UnitArray.Add(newUnit);
    }


    cout<<endl<< "Iterator"<<endl;
    Iterator<Units*> *allIt = UnitArray.GetIterator();
    AllUnit(allIt);
    delete allIt;

    cout << endl << "Change only Necromancer Units:" << endl;
    Iterator<Units*> *NecroIt = new UnitFactionDecorator(UnitArray.GetIterator(), UnitFaction::Necropolis);
    AllUnit(NecroIt);
    delete NecroIt;

    cout << endl << "Change only Units with normal hp" << endl;
    Iterator<Units*> *HpIt = new UnitHealDecorator(UnitArray.GetIterator(),3);
    AllUnit(HpIt);
    delete HpIt;

    cout<< endl<< "Dmg normal units" <<endl;
    Iterator<Units*> *DmgIt = new UnitDmgDecorator(new UnitHealDecorator(UnitArray.GetIterator(),3));
    AllUnit(DmgIt);
    delete DmgIt;








    cout<<endl<<endl;
    vector<Units*> UnitVector;
    for(size_t i=0; i<5; i++)
    {
        int unit_num = rand()%3+1;
        UnitType unit_type = static_cast<UnitType>(unit_num);
        Units *newUnit = CreateUnit(unit_type);
        UnitVector.push_back(newUnit);
    }
    cout<<"STL"<<endl;
    Iterator<Units*> *adaptedIt = new ConstIteratorAdapter<vector<Units*>, Units*>(&UnitVector);
    Iterator<Units*> *adaptedHealIt = new UnitHealDecorator(adaptedIt,3);
    AllUnit(adaptedHealIt);
    delete adaptedHealIt;





    return 0;
}
