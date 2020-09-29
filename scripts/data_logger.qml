import QtQuick 2.0
import VeinEntity 1.0
import VeinLogger 1.0

VeinLogger {
    initializeValues: true;
    recordName: loggerEntity.recordName;
    transactionName: loggerEntity.transactionName;


    readonly property QtObject systemEntity: VeinEntity.getEntity("_System");
    readonly property QtObject loggerEntity: VeinEntity.getEntity("_LoggingSystem");
    readonly property string sysSession: systemEntity.Session
    onSysSessionChanged: {
        session = systemEntity.Session;
        loggerEntity.availableContentSets = readSession();
    }

    readonly property string sysContentSet: loggerEntity.currentContentSet;
    onSysContentSetChanged: {
        // update VeinLogger property contentSet
        contentSet = loggerEntity.currentContentSet;

        var loggedComponentsFromContentSet = readContentSet();

        clearLoggerEntries();
        systemEntity.LoggedComponents = loggedComponentsFromContentSet;
    }

    readonly property bool scriptRunning: loggingEnabled
    onScriptRunningChanged: {
        if(scriptRunning === true) {
            console.log("starting logging at", new Date().toLocaleTimeString());
            startLogging();
        }
        else {
            stopLogging();
            console.log("stopped logging at", new Date().toLocaleTimeString());
        }
    }

    // Still makes sense to view/access LoggedComponents
    readonly property var loggedValues: systemEntity.LoggedComponents
    onLoggedValuesChanged: {
        clearLoggerEntries();
        for(var entityID in loggedValues) {
            var componentsArray = loggedValues[entityID];
            for(var componentConfigured = 0; componentConfigured < componentsArray.length; ++componentConfigured) {
                addLoggerEntry(entityID, componentsArray[componentConfigured]);
            }
        }
    }
}
