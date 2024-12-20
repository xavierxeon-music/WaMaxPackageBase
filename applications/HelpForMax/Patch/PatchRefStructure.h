#ifndef PatchRefStructureH
#define PatchRefStructureH

#include <QObject>

#include <QIcon>
#include <QMap>
#include <QString>
#include <QStringList>

#include "MaxDataType.h"

namespace Patch
{
   struct Info
   {
      QString name;
      QString folder;
   };

   class RefStructure
   {
      Q_GADGET

   public:
      enum class PatchType
      {
         Standard,
         Gui,
         Poly,
         Fourier
      };
      Q_ENUM(PatchType)

      enum class PatchPart
      {
         Undefined = 0x00,
         Header = 0x01,
         Argument = 0x02,
         Attribute = 0x04,
         MessageTyped = 0x08,
         MessageNamed = 0x10,
         Output = 0x20,
         Other = 0x40
      };
      Q_ENUM(PatchPart)
      Q_DECLARE_FLAGS(PatchParts, PatchPart)

      static const QList<QByteArray> descriptionMaxTags;

      struct Digest
      {
         QString text;
         QString description;
      };

      struct Header
      {
         Digest digest;
         PatchType patcherType = PatchType::Standard;

         QStringList metaTagList;
         QStringList seeAlsoList;
      };

      struct Output
      {
         Max::DataType dataType = Max::DataType::Anything;
         bool active = false;
         Digest digest;

         using List = QList<Output>;
         using Map = QMap<Max::DataType, Output>;
      };

      struct AbstractMessage
      {
         QString name;
         Max::DataType dataType = Max::DataType::Symbol;
         Digest digest;
      };

      struct MessageTyped : AbstractMessage
      {
         bool active = false;

         using List = QList<MessageTyped>;
         using Map = QMap<Max::DataType, MessageTyped>;
      };

      // attributes and things in patcherargs with @
      struct AttributesAndMessageNamed : AbstractMessage
      {
         PatchParts patchParts = PatchPart::Undefined;

         using List = QList<AttributesAndMessageNamed>;
         using Map = QMap<QString, AttributesAndMessageNamed>;
      };

      // things in patcherargs without @
      struct Argument : AbstractMessage
      {
         bool optional = false;

         using List = QList<Argument>;
      };

      using SeeAlsoList = QStringList;

   public:
      RefStructure();
      virtual ~RefStructure();

   public:
      virtual void clear();
      virtual void setDirty();

      // patcher type
      static QString patchTypeName(const PatchType& type);
      static PatchType toPatchType(const QString& name);
      static QList<PatchType> patchTypeList();

      // part
      static QString partName(const PatchPart& part);
      static QIcon partIcon(const PatchPart& part);
      static PatchPart toPart(const QString& name);

      void repackNamedMessages();

   public:
      Header header;
      Output::Map outputMap;
      Argument::List argumentList;
      MessageTyped::Map messageTypedMap;
      AttributesAndMessageNamed::Map messageNamedMap;
   };
} // namespace Patch

Q_DECLARE_OPERATORS_FOR_FLAGS(Patch::RefStructure::PatchParts)

#endif // NOT PatchRefStructureH
