#include "qtzldialogmanager.h"
#include <ZLDialog.h>
#include <ZLProgressDialog.h>
#include <ZLOptionsDialog.h>
#include <ZLTreeDialog.h>

QtZLDialogManager::QtZLDialogManager()
{
}

void QtZLDialogManager::createInstance()
{
    ourInstance = new QtZLDialogManager;
}

shared_ptr<ZLDialog> QtZLDialogManager::createDialog(const ZLResourceKey &key) const
{
    (void) key;
    return nullptr;
}

shared_ptr<ZLOpenFileDialog> QtZLDialogManager::createOpenFileDialog(const ZLResourceKey &key, const std::string &directoryPath, const std::string &filePath, const ZLOpenFileDialog::Filter &filter) const
{
    (void) key;
    (void) directoryPath;
    (void) filePath;
    (void) filter;
    return nullptr;
}

void QtZLDialogManager::informationBox(const std::string &title, const std::string &message) const
{
    (void) title;
    (void) message;
}

void QtZLDialogManager::errorBox(const ZLResourceKey &key, const std::string &message) const
{
    (void) key;
    (void) message;
}

int QtZLDialogManager::questionBox(const ZLResourceKey &key, const std::string &message, const ZLResourceKey &button0, const ZLResourceKey &button1, const ZLResourceKey &button2) const
{
    (void) key;
    (void) message;
    (void) button0;
    (void) button1;
    (void) button2;
    return 0;
}

bool QtZLDialogManager::isClipboardSupported(ZLDialogManager::ClipboardType type) const
{
    (void) type;
    return false;
}

void QtZLDialogManager::setClipboardText(const std::string &text, ZLDialogManager::ClipboardType type) const
{
    (void) text;
    (void) type;
}

void QtZLDialogManager::setClipboardImage(const ZLImageData &imageData, ZLDialogManager::ClipboardType type) const
{
    (void) imageData;
    (void) type;
}


void QtZLDialogManager::showSearchBox() const
{
}

shared_ptr<ZLOptionsDialog> QtZLDialogManager::createOptionsDialog(const ZLResourceKey &key, shared_ptr<ZLRunnable> applyAction) const
{
    (void) key;
    (void) applyAction;
    return nullptr;
}

shared_ptr<ZLTreeDialog> QtZLDialogManager::createTreeDialog(const std::string &windowName, const ZLResource &resource) const
{
    (void) windowName;
    (void) resource;
    return nullptr;
}

class QtZLProgressDialogDummy : public ZLProgressDialog
{
public:
    QtZLProgressDialogDummy(const ZLResourceKey &key) : ZLProgressDialog(key)
    {
    }

    void run(ZLRunnable &runnable)
    {
        runnable.run();
    }

    void setMessage(const std::string &message)
    {
        (void) message;
    }
};

shared_ptr<ZLProgressDialog> QtZLDialogManager::createProgressDialog(const ZLResourceKey &key, bool network) const
{
    (void) network;
    return new QtZLProgressDialogDummy(key);
}
