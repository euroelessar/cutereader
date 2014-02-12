#include "qtzlibrary.h"
#include "qtzlfsmanager.h"
#include "qtzltimemanager.h"
#include "qtzlconfigmanager.h"
#include "qtzlimagemanager.h"
#include "qtzlencodingconverterprovider.h"
#include "qtzldialogmanager.h"
#include "qtzlworker.h"
#include <ZLOptions.h>
#include <ZLEncodingConverter.h>
#include <QLocale>
#include <QStandardPaths>
#include <QGuiApplication>
#include <QDesktopServices>
#include <QScreen>

const std::string ZLibrary::FileNameDelimiter = "/";
const std::string ZLibrary::PathDelimiter = ":";
const std::string ZLibrary::EndOfLine = "\n";
const std::string ZLibrary::BaseDirectory;

bool ZLibrary::ourLocaleIsInitialized = false;
std::string ZLibrary::ourLanguage;
std::string ZLibrary::ourCountry;
std::string ZLibrary::ourZLibraryDirectory;

std::string ZLibrary::ourImageDirectory;
std::string ZLibrary::ourApplicationImageDirectory;
std::string ZLibrary::ourApplicationName;
std::string ZLibrary::ourApplicationDirectory;
std::string ZLibrary::ourApplicationWritableDirectory;
std::string ZLibrary::ourDefaultFilesPathPrefix;

bool ZLibrary::init(int &argc, char **&argv)
{
    parseArguments(argc, argv);

    QString locale = QLocale::system().name();
    ourLanguage = locale.section(QLatin1Char('_'), 0, 0).toStdString();
    ourCountry = locale.section(QLatin1Char('_'), 1, 1).toStdString();
    ourLocaleIsInitialized = true;

    ourZLibraryDirectory = QtZLFSManager::dataPath().toStdString();
    ourApplicationImageDirectory = QStandardPaths::writableLocation(QStandardPaths::CacheLocation).toStdString() + "/images";
    ourApplicationName = qApp->applicationName().toStdString();
    ourApplicationDirectory = QtZLFSManager::dataPath().toStdString();
    ourApplicationWritableDirectory = QStandardPaths::writableLocation(QStandardPaths::DataLocation).toStdString();
    ourDefaultFilesPathPrefix = QtZLFSManager::dataPath().toStdString();
    
    qDebug("data locations: \"%s\"", qPrintable(QStandardPaths::standardLocations(QStandardPaths::DataLocation).join("\", \"")));
    qDebug("ourApplicationWritableDirectory: \"%s\"", ourApplicationWritableDirectory.c_str());
    qDebug("ourDefaultFilesPathPrefix: \"%s\"", ourDefaultFilesPathPrefix.c_str());

    QtZLFSManager::createInstance();
    QtZLTimeManager::createInstance();
    QtZLConfigManager::createInstance();
    QtZLImageManager::createInstance();
    QtZLDialogManager::createInstance();
    ZLEncodingCollection::Instance().registerProvider(new QtZLEncodingConverterProvider);
    ZLOption::createInstance();
    QtZLWorker::createInstance();

    return true;
}

void ZLibrary::parseArguments(int &argc, char **&argv)
{
    (void) argc;
    (void) argv;
}

ZLPaintContext *ZLibrary::createContext()
{
    return nullptr;
}

void ZLibrary::run(ZLApplication *application)
{
    (void) application;
}

std::string ZLibrary::Language()
{
    return ourLanguage;
}

std::string ZLibrary::Country()
{
    return ourCountry;
}

void ZLibrary::shutdown()
{
    QtZLWorker::deleteInstance();
}

void ZLibrary::initLocale()
{
}

std::string ZLibrary::replaceRegExps(const std::string &pattern)
{
    return pattern;
}

void ZLibrary::initApplication(const std::string &name)
{
    (void) name;
}

ZLibrary::ZLibrary()
{
}

bool ZLibrary::openUrl(const std::string &url)
{
    return QDesktopServices::openUrl(QUrl(QString::fromStdString(url)));
}

std::size_t ZLibrary::displayDPI()
{
    QScreen *screen = QGuiApplication::screens().value(0);
    return screen->logicalDotsPerInch();
}

std::size_t ZLibrary::displayPixelsHeight()
{
    QScreen *screen = QGuiApplication::screens().value(0);
    return screen->availableSize().height();
}

std::size_t ZLibrary::displayPixelsWidth()
{
    QScreen *screen = QGuiApplication::screens().value(0);
    return screen->availableSize().width();
}
