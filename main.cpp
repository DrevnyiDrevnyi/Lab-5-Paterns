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

enum class LvlUp : int
{
    Opolchenec,
    SkeletonArcher,
    MasterGremlin,

    None
};

class UpStrategy
{
public:
    virtual ~UpStrategy() {}
    virtual int GetSell () = 0;

};

class OpolchenecUp : public UpStrategy
{
    int GetSell() {cout << "Peasant can be up into Opolchenec";}
};
class ArcherUp : public UpStrategy
{
    int GetSell() {cout << "Skeleton can be up into Skeleton_Archer";}
};
class MasterUp : public UpStrategy
{
    int GetSell() {cout << "Gremlin can be up into Master_Gremlin ";}
};

UpStrategy *CreateUp (LvlUp typeUp)
{
    switch(typeUp)
    {
        case LvlUp::Opolchenec : return new OpolchenecUp;
        case LvlUp::SkeletonArcher : return new ArcherUp;
        case LvlUp::MasterGremlin : return new MasterUp;

        default: return nullptr;
    }
}

class Units
{
private:
    UnitFaction Faction;
    int Tier;

    UpStrategy *UpManner;
    void DoUp()
    {
        if (UpManner == nullptr)
        {
            cout<<"nothing";
            return;
        }
        else
        {
            UpManner -> GetSell();
        }
    }

protected:
    int Heal;
    int Sell;
    double Dmg;
public:
    Units (UnitFaction faction): Faction(faction), Tier(1), Heal(1), Sell(1), Dmg(1.0)
    {
        cout<< "Unit from your fraction is ready" << endl;
    }
    virtual ~Units(){if (UpManner != nullptr) delete UpManner; }

    virtual int GetHeal() {return Heal;}
    virtual double GetDmg() {return Dmg;}
    UnitFaction GetFaction() const {return Faction;}
    virtual void UnitName() = 0;
    virtual int GetSell()
    {
        UnitName();
        cout<< " : ";
        DoUp();
        cout<< " Sell: "<<Sell<<" HP: " <<Heal<< " Dmg: " << Dmg;

        cout<<endl;
    }
    void SetUpManner(UpStrategy *upManner) { UpManner = upManner; }

};


class Gremlin : public Units
{
private:
    int Proj;
public:
    Gremlin();
    ~Gremlin(){}
    int GetProj(){}
    void UnitName() {cout<< "Gremlin";}
};
Gremlin ::Gremlin() : Units (UnitFaction ::Academy)
{
    SetUpManner (CreateUp (LvlUp::MasterGremlin));
    Dmg = rand()%2+1;
    Heal =rand()%5+1;
    Sell = 22;
    cout <<"Gremlin is buy" << endl;
}

class Peasant : public Units
{
public:
    Peasant();
    ~Peasant(){}
    void UnitName() {cout<< "Peasant";}
};
Peasant ::Peasant() : Units (UnitFaction ::Orden)
{
    SetUpManner (CreateUp (LvlUp::Opolchenec));
    Dmg = 1.0;
    Heal = rand()%3+1;
    Sell = 15;
    cout <<"Peasant is buy" << endl;
}

class Skeleton : public Units
{
public:
    Skeleton();
    ~Skeleton(){}
    void UnitName() {cout<< "Skeleton";}
};
Skeleton ::Skeleton() : Units (UnitFaction :: Necropolis)
{
    SetUpManner (CreateUp (LvlUp::SkeletonArcher));
    Dmg = 1.0;
    Sell = 19;
    Heal = rand()%4+1;
    cout <<"Peasant became a Skeleton" << endl;
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
