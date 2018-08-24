// Copyright 2018 The Beam Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <QApplication>
#include <QtQuick>

#include <QInputDialog>
#include <QMessageBox>

#include <qqmlcontext.h>
#include "viewmodel/start.h"
#include "viewmodel/main.h"
#include "viewmodel/utxo.h"
#include "viewmodel/dashboard.h"
#include "viewmodel/address_book.h"
#include "viewmodel/wallet.h"
#include "viewmodel/notifications.h"
#include "viewmodel/help.h"
#include "viewmodel/settings.h"
#include "viewmodel/messages.h"
#include "model/app_model.h"

#include "wallet/wallet_db.h"
#include "utility/logger.h"
#include "core/ecc_native.h"

#include "translator.h"

#include "utility/options.h"

#include <QtCore/QtPlugin>

#include "version.h"

#include "utility/string_helpers.h"

#if defined(BEAM_USE_STATIC)

#if defined Q_OS_WIN
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#elif defined Q_OS_MAC
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
#elif defined Q_OS_LINUX
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
#endif

Q_IMPORT_PLUGIN(QtQuick2Plugin)
Q_IMPORT_PLUGIN(QtQuick2WindowPlugin)
Q_IMPORT_PLUGIN(QtQuickControls1Plugin)
Q_IMPORT_PLUGIN(QtQuickControls2Plugin)
Q_IMPORT_PLUGIN(QtGraphicalEffectsPlugin)
Q_IMPORT_PLUGIN(QtGraphicalEffectsPrivatePlugin)
Q_IMPORT_PLUGIN(QSvgPlugin)
Q_IMPORT_PLUGIN(QtQuickLayoutsPlugin)
Q_IMPORT_PLUGIN(QtQuickTemplates2Plugin)

#endif

using namespace beam;
using namespace std;
using namespace ECC;

int main (int argc, char* argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("Beam Wallet");

    QDir appDataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

    try
    {
        po::options_description options = createOptionsDescription();
        po::variables_map vm;

        try
        {
            vm = getOptions(argc, argv, "beam-wallet.cfg", options);
        }
        catch (const po::error& e)
        {
            cout << e.what() << std::endl;
            cout << options << std::endl;

            return -1;
        }

        if (vm.count(cli::HELP))
        {
            cout << options << std::endl;

            return 0;
        }

        if (vm.count(cli::VERSION))
        {
            cout << PROJECT_VERSION << endl;
            return 0;
        }

        if (vm.count(cli::GIT_COMMIT_HASH))
        {
            cout << GIT_COMMIT_HASH << endl;
            return 0;
        }

        if (vm.count(cli::APPDATA_PATH))
        {
            appDataDir = QString::fromStdString(vm[cli::APPDATA_PATH].as<string>());
        }

        int logLevel = getLogLevel(cli::LOG_LEVEL, vm, LOG_LEVEL_DEBUG);
        int fileLogLevel = getLogLevel(cli::FILE_LOG_LEVEL, vm, LOG_LEVEL_INFO);
#if LOG_VERBOSE_ENABLED
        logLevel = LOG_LEVEL_VERBOSE;
#endif
        
        auto logger = beam::Logger::create(logLevel, logLevel, fileLogLevel, "beam_ui_", appDataDir.filePath("./logs").toStdString());

        try
        {
            Rules::get().UpdateChecksum();
            LOG_INFO() << "Rules signature: " << Rules::get().Checksum;

            auto walletStorage = appDataDir.filePath("wallet.db").toStdString();
            auto bbsStorage = appDataDir.filePath("keys.bbs").toStdString();

            QQuickView view;
            view.setResizeMode(QQuickView::SizeRootObjectToView);
            view.setMinimumSize(QSize(860, 700));
            WalletSettings settings(appDataDir.filePath("setting.ini"));
            AppModel appModel(settings);

            settings.setWalletStorage(walletStorage);
            settings.setBbsStorage(bbsStorage);

            if (settings.getNodeAddress().isEmpty())
            {
                if (vm.count(cli::NODE_ADDR))
                {
                    string nodeAddr = vm[cli::NODE_ADDR].as<string>();
                    settings.setNodeAddress(nodeAddr.c_str());
                }
            }

            qmlRegisterType<StartViewModel>("Beam.Wallet", 1, 0, "StartViewModel");
            qmlRegisterType<MainViewModel>("Beam.Wallet", 1, 0, "MainViewModel");
            qmlRegisterType<DashboardViewModel>("Beam.Wallet", 1, 0, "DashboardViewModel");
            qmlRegisterType<WalletViewModel>("Beam.Wallet", 1, 0, "WalletViewModel");
            qmlRegisterType<UtxoViewModel>("Beam.Wallet", 1, 0, "UtxoViewModel");
            qmlRegisterType<SettingsViewModel>("Beam.Wallet", 1, 0, "SettingsViewModel");
            qmlRegisterType<AddressBookViewModel>("Beam.Wallet", 1, 0, "AddressBookViewModel");
            qmlRegisterType<NotificationsViewModel>("Beam.Wallet", 1, 0, "NotificationsViewModel");
            qmlRegisterType<HelpViewModel>("Beam.Wallet", 1, 0, "HelpViewModel");
            qmlRegisterType<MessagesViewModel>("Beam.Wallet", 1, 0, "MessagesViewModel");

            qmlRegisterType<PeerAddressItem>("Beam.Wallet", 1, 0, "PeerAddressItem");
            qmlRegisterType<OwnAddressItem>("Beam.Wallet", 1, 0, "OwnAddressItem");
            qmlRegisterType<TxObject>("Beam.Wallet", 1, 0, "TxObject");
            qmlRegisterType<UtxoItem>("Beam.Wallet", 1, 0, "UtxoItem");

            Translator translator;
            view.setSource(QUrl("qrc:///root.qml"));

            view.show();

            return app.exec();
        }
        catch (const po::error& e)
        {
            LOG_ERROR() << e.what();
            return -1;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }
}