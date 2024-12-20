#include "PatchModelArgument.h"

Patch::Model::Argument::Argument(QObject* parent, RefStructure* structure)
   : Abstract(parent, structure, RefStructure::PatchPart::Argument)
   , Delegate::DataType::Source()
{
}

void Patch::Model::Argument::update()
{
   for (int row = 0; row < invisibleRootItem()->rowCount(); row++)
   {
      QStandardItem* nameItem = invisibleRootItem()->child(row, 0);
      QStandardItem* typeItem = invisibleRootItem()->child(row, 1);
      QStandardItem* optionalItem = invisibleRootItem()->child(row, 2);
      QStandardItem* digestItem = invisibleRootItem()->child(row, 3);

      const RefStructure::Argument& argument = structure->argumentList.at(row);

      nameItem->setText(argument.name);
      typeItem->setText(Max::dataTypeName(argument.dataType));
      optionalItem->setCheckState(argument.optional ? Qt::Checked : Qt::Unchecked);

      updateDigestItem(digestItem, argument.digest);
   }

   emit signalDataEdited();
}

void Patch::Model::Argument::rebuild()
{
   beginResetModel();
   setHorizontalHeaderLabels({"Name", "Type", "Optional", "Digest"});

   for (int row = 0; row < structure->argumentList.count(); row++)
   {
      QStandardItem* nameItem = new QStandardItem();

      QStandardItem* typeItem = new QStandardItem();

      QStandardItem* optionalItem = new QStandardItem();
      optionalItem->setCheckable(true);
      optionalItem->setEditable(false);
      optionalItem->setIcon(QIcon(":/DocArgument.svg"));

      QStandardItem* digestItem = new QStandardItem();
      digestItem->setEditable(false);

      invisibleRootItem()->appendRow({nameItem, typeItem, optionalItem, digestItem});
   }

   endResetModel();

   update();
}

Patch::RefStructure::Digest* Patch::Model::Argument::getDigest(const QModelIndex& index)
{
   RefStructure::Argument& argument = structure->argumentList[index.row()];
   return &(argument.digest);
}

void Patch::Model::Argument::createBeforeItem(const QModelIndex& index)
{
   RefStructure::Argument argument;
   argument.name = "???";

   if (index.isValid())
      structure->argumentList.insert(index.row(), argument);
   else
      structure->argumentList.append(argument);

   structure->setDirty();
}

void Patch::Model::Argument::removeItem(const QModelIndex& index)
{
   structure->argumentList.removeAt(index.row());
   structure->setDirty();
}

bool Patch::Model::Argument::setData(const QModelIndex& index, const QVariant& value, int role)
{
   const bool result = QStandardItemModel::setData(index, value, role);
   RefStructure::Argument& argument = structure->argumentList[index.row()];

   if (Qt::EditRole == role)
   {
      if (0 == index.column())
      {
         argument.name = value.toString();
         structure->setDirty();
      }
      else if (1 == index.column())
      {
         argument.dataType = Max::toDataType(value.toString());
         structure->setDirty();
      }
   }
   else if (Qt::CheckStateRole == role)
   {
      argument.optional = (Qt::Checked == value.toInt());
      structure->setDirty();
   }

   emit signalDataEdited();
   return result;
}

Max::DataType Patch::Model::Argument::getDataType(const int index)
{
   const RefStructure::Argument& argument = structure->argumentList.at(index);

   return argument.dataType;
}
