#include "MaxDataType.h"

static const Max::DataTypeNameMap dataTypeNameMap = {{Max::DataType::Anything, "anything"}, // must be max names!
                                                     {Max::DataType::Bang, "bang"},
                                                     {Max::DataType::Integer, "int"},
                                                     {Max::DataType::Float, "float"},
                                                     {Max::DataType::Symbol, "symbol"},
                                                     {Max::DataType::List, "list"},
                                                     {Max::DataType::Signal, "signal"},
                                                     {Max::DataType::MultiSignal, "multichannelsignal"},
                                                     {Max::DataType::Dictionary, "dictionary"},
                                                     {Max::DataType::Matrix, "matrix"}};

QString Max::dataTypeName(const DataType& type)
{
   return dataTypeNameMap.value(type, "anything");
}

Max::DataType Max::toDataType(const QString& name)
{
   for (DataTypeNameMap ::const_iterator it = dataTypeNameMap.constBegin(); it != dataTypeNameMap.constEnd(); it++)
   {
      if (name == it.value())
         return it.key();
   }

   return DataType::Anything;
}

QList<Max::DataType> Max::dataTypeList()
{
   return dataTypeNameMap.keys();
}