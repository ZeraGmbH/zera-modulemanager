import QtQuick 2.0
import VeinEntity 1.0
import VeinLogger 1.0

VeinLogger {
  recordName: "default";
  initializeValues: true;

  readonly property QtObject systemEntity: VeinEntity.getEntity("_System");
  readonly property string session: systemEntity.Session
  readonly property bool scriptRunning: loggingEnabled && session === "0_default-session.json"

  readonly property var loggedValues: systemEntity.LoggedComponents
  onLoggedValuesChanged: {
    console.log("TESTX1", loggedValues);
    clearLoggerEntries();
    for(var it in loggedValues)
    {
      var entData = loggedValues[it];
      console.log(it, entData.length)
      for(var i=0; i<entData.length; ++i)
      {
        console.log("\t", i, entData[i]);
        addLoggerEntry(it, entData[i]);
      }
    }
  }

  function initScript() {
    var logValues = {};
    //Range module 1001
    var rangeModuleEntries = [];
    rangeModuleEntries.push("ACT_Frequency");
    rangeModuleEntries.push("PAR_Channel1Range");
    rangeModuleEntries.push("PAR_Channel2Range");
    rangeModuleEntries.push("PAR_Channel3Range");
    rangeModuleEntries.push("PAR_Channel4Range");
    rangeModuleEntries.push("PAR_Channel5Range");
    rangeModuleEntries.push("PAR_Channel6Range");
    logValues["1001"] = rangeModuleEntries;
    //RMS module 1003
    var rmsModuleEntries = [];
    rmsModuleEntries.push("ACT_RMSPN1");
    rmsModuleEntries.push("ACT_RMSPN2");
    rmsModuleEntries.push("ACT_RMSPN3");
    rmsModuleEntries.push("ACT_RMSPN4");
    rmsModuleEntries.push("ACT_RMSPN5");
    rmsModuleEntries.push("ACT_RMSPN6");
    rmsModuleEntries.push("ACT_RMSPP1");
    rmsModuleEntries.push("ACT_RMSPP2");
    rmsModuleEntries.push("ACT_RMSPP3");
    logValues["1003"] = rmsModuleEntries;
    //DFT module 1004
    var dftModuleEntries = [];
    dftModuleEntries.push("ACT_DFTPN1");
    dftModuleEntries.push("ACT_DFTPN2");
    dftModuleEntries.push("ACT_DFTPN3");
    dftModuleEntries.push("ACT_DFTPN4");
    dftModuleEntries.push("ACT_DFTPN5");
    dftModuleEntries.push("ACT_DFTPN6");
    dftModuleEntries.push("ACT_DFTPP1");
    dftModuleEntries.push("ACT_DFTPP2");
    dftModuleEntries.push("ACT_DFTPP3");
    logValues["1004"] = dftModuleEntries;
    //Power1Module1 1006
    var p1m1Entries = [];
    p1m1Entries.push("ACT_PQS1");
    p1m1Entries.push("ACT_PQS2");
    p1m1Entries.push("ACT_PQS3");
    p1m1Entries.push("ACT_PQS4");
    p1m1Entries.push("PAR_MeasuringMode");
    logValues["1006"] = p1m1Entries;
    //Power1Module2 1007
    var p1m2Entries = [];
    p1m2Entries.push("ACT_PQS1");
    p1m2Entries.push("ACT_PQS2");
    p1m2Entries.push("ACT_PQS3");
    p1m2Entries.push("ACT_PQS4");
    p1m2Entries.push("PAR_MeasuringMode");
    logValues["1007"] = p1m2Entries;
    //Power1Module3 1008
    var p1m3Entries = [];
    p1m3Entries.push("ACT_PQS1");
    p1m3Entries.push("ACT_PQS2");
    p1m3Entries.push("ACT_PQS3");
    p1m3Entries.push("ACT_PQS4");
    p1m3Entries.push("PAR_MeasuringMode");
    logValues["1008"] = p1m3Entries;

    systemEntity.LoggedComponents = logValues;
  }

  Component.onCompleted: initScript();

  onScriptRunningChanged: {
    if(scriptRunning === true)
    {
      console.log("starting logging at", new Date().toLocaleTimeString());
      startLogging();
    }
    else
    {
      console.log("stopped logging at", new Date().toLocaleTimeString());
      stopLogging();
    }
  }
}
