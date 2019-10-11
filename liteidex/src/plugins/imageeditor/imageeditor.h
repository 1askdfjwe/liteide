#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include "liteapi/liteapi.h"

class ImageEditorFile;
class ImageEditorWidget;
class ImageEditor : public LiteApi::IEditor
{
public:
    ImageEditor(LiteApi::IApplication *app);
    virtual ~ImageEditor();
    virtual QWidget *widget();
    virtual QString name() const;
    virtual bool open(const QString &filePath,const QString &mimeType);
    virtual bool reload();
    virtual bool save();
    virtual bool saveAs(const QString &filePath);
    virtual void setReadOnly(bool b);
    virtual bool isReadOnly() const;
    virtual bool isModified() const;
    virtual QString filePath() const;
    virtual QString mimeType() const;
    virtual QByteArray saveState() const;
    virtual bool restoreState(const QByteArray &array);
    virtual void onActive();
protected:
    LiteApi::IApplication *m_liteApp;
    ImageEditorFile *m_file;
    ImageEditorWidget *m_widget;
};


#endif // IMAGEEDITOR_H
