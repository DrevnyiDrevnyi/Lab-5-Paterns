#ifndef FILL_H_INCLUDED
#define FILL_H_INCLUDED
#include <vector>
using namespace std;


template<typename T>
class Iterator
{
protected:
    Iterator() {}

public:
    virtual ~Iterator() {}
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual T GetCurrent() const = 0;
};

template<typename T>
class ArrayIterator : public Iterator<T>
{
private:

    const T *ArrayContainer;
    size_t Pos;
    size_t Size;

public:
    ArrayIterator(T *container, size_t size)
    : ArrayContainer(container), Pos(0), Size(size) {}

    void First() override { Pos = 0; }
    void Next() override { Pos++; }
    bool IsDone() const override { return (Pos >= Size); }
    T GetCurrent() const override { return ArrayContainer[Pos]; }
};
const size_t MaxSize = 100;

template <typename T>
class ArrayClass
{
private:
    T Items[MaxSize];
    size_t ArraySize;

public:
    void Add(T newObject) { Items[ArraySize++] = newObject; }
    size_t Size() const { return ArraySize; }
    T GetElement(size_t index) const { return Items[index]; }

    class Iterator<T> *GetIterator()
    {
        return new ArrayIterator<T>(Items, ArraySize);
    };

    ArrayClass() : ArraySize(0) {}
};



template<typename T>
class VectorUnitIterator : public Iterator<T>
{
private:
    const T *VectorUnitContainer;
    size_t VecU;
    size_t VecSize;

public:
    VectorUnitIterator(T *container, size_t size)
    : VectorUnitContainer(container), VecU(0), VecSize(size) {}

    void First() override { VecU = 0; }
    void Next() override { VecU++; }
    bool IsDone() const override { return (VecU >= VecSize); }
    T GetCurrent() const override { return VectorUnitContainer[VecU]; }

};

template <typename T>
class VectorClassUnit
{
private:
    vector<T> Items;

public:
    void Push(T newObject) { Items.push_back(newObject); }
    size_t Size() const { return Items.size(); }
    T GetVectorElement(size_t index) const { return Items[index]; }
    class Iterator<T> *GetIterator()
    {
        return new VectorUnitIterator<T>(Items);
    };
    VectorClassUnit() {};
};


template<typename T>
class IteratorDecorator : public Iterator<T>
{
protected:
    Iterator<T> *It;

public:
    IteratorDecorator(Iterator<T> *it) : It(it) {}
    virtual ~IteratorDecorator() { delete It; }
    virtual void First() { It->First(); }
    virtual void Next() { It->Next(); }
    virtual bool IsDone() const { return It->IsDone(); }
    virtual T GetCurrent() const { return It->GetCurrent(); }
};

template<typename ContainerType, typename ItemType>
class ConstIteratorAdapter : public Iterator<ItemType>
{
protected:
    ContainerType *Container;
    typename ContainerType::const_iterator It;

public:
    ConstIteratorAdapter(ContainerType *container)
    : Container(container)
    {
        It = Container->begin();
    }

    virtual ~ConstIteratorAdapter() {}
    virtual void First() { It = Container->begin(); }
    virtual void Next() { It++; }
    virtual bool IsDone() const { return (It == Container->end()); }
    virtual ItemType GetCurrent() const { return *It; }
};


#endif // FILL_H_INCLUDED
