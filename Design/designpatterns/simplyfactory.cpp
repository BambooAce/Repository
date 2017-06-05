#include <iostream>
#include <vector>
using namespace std;

typedef enum ProductTypeTag
{
  TypeA,
  TypeB,
  TypeC
} PRODUCTTYPE;

// Here is the product class
class Product
{
public:
  virtual void Show () = 0;
};

class ProductA:public Product
{
public:
  void Show ()
  {
    cout << "I'm ProductA" << endl;
  }
};

class ProductB:public Product
{
public:
  void Show ()
  {
    cout << "I'm ProductB" << endl;
  }
};

class ProductC:public Product
{
public:
  void Show ()
  {
    cout << "I'm ProductC" << endl;
  }
};

// Here is the Factory class
class Factory
{
public:
  Product * CreateProduct (PRODUCTTYPE type)
  {
    switch (type)
      {
      	case TypeA:
			return new ProductA ();

		case TypeB:
			return new ProductB ();

		case TypeC:
			return new ProductC ();

		default:
			return NULL;
      }
  }
};

int
main (int argc, char *argv[])
{
  // First, create a factory object
  Factory *ProductFactory = new Factory ();
  Product *productObjA = ProductFactory->CreateProduct (TypeA);
  if (productObjA != NULL)
    productObjA->Show ();

  Product *productObjB = ProductFactory->CreateProduct (TypeB);
  if (productObjB != NULL)
    productObjB->Show ();

  Product *productObjC = ProductFactory->CreateProduct (TypeC);
  if (productObjC != NULL)
    productObjC->Show ();

  delete ProductFactory;
  ProductFactory = NULL;

  delete productObjA;
  productObjA = NULL;

  delete productObjB;
  productObjB = NULL;

  delete productObjC;
  productObjC = NULL;

  return 0;
}

