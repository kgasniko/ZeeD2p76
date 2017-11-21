#include "ZeeDAnalysis/ZeeDPlotter.h"

// Root includes
//#include <TUnixSystem.h>

// Athena includes
//#include "GaudiKernel/MsgStream.h"
//#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IToolSvc.h"

// ZeeD analysis includes
#include "ZeeDHist/ZeeDHistPlotter.h"
#include "ZeeDHist/ZeeDHistArray.h"

#include "ZeeDHist/ZeeDHistParser.h"

#include "ZeeDCuts/ZeeDCutHistManager.h"

#include <TKey.h>
#include <TCanvas.h>
#include <TPostScript.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TMath.h>
#include <TSystem.h>
#include "RooFit.h"
#include "RooGlobalFunc.h"
#include <iostream>
#include <string> 
#include <map>
#include <sys/types.h>
#include <dirent.h>

//RooFit includes
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooMsgService.h"

using namespace RooFit;
using std::vector;
using std::string;
using std::string;
using std::cout;
using std::endl;


//------------------------------------------------------
ZeeDPlotter::ZeeDPlotter(const std::string& name,
        ISvcLocator* pSvcLocator) : AthAlgorithm(name, pSvcLocator),
    bDrawOverlayPlotsW(kFALSE),
    EffBinGridTop(NULL),
    EffBinGridBot(NULL),
    EffBinGrid(NULL),
    ErrEffBinGrid(NULL)

{
    // Constructor
    declareProperty( "FileNames", m_filenames );
    declareProperty( "DirNames",  m_dirnames );

    declareProperty( "DataOperation", m_dataoperation = "");
    declareProperty( "McOperation", m_mcoperation);

    declareProperty( "CutSelector", m_cutselector);
    declareProperty( "CutFlow", m_cutflow);
    declareProperty( "DataName",m_dataname = "out.root");

    declareProperty( "Luminosity", m_lumi = 3948.03);
    declareProperty( "LuminosityError", m_lumi_err = 0.031);

    declareProperty( "SignalMCName",m_mcname = "");
    declareProperty( "SystematicsList",m_systlist);
    declareProperty( "BosonSelector",m_selectonCZ);    
    declareProperty( "RefHistogramRec", m_refHistogramRec = "Boson/BosY");
    declareProperty( "RefHistogramRecToy", m_refHistogramRecToy = "BosY");
    declareProperty( "RefHistogramGen", m_refHistogramGen = "BosY");    
    declareProperty( "RefHistogramGenToy", m_refHistogramGenToy = "BosMt");    
    declareProperty( "GeneratorHistogram", m_GenHist = "BosY" );

    declareProperty( "SetLogPlots", m_logscale   = false );
    declareProperty( "SetNormalize", m_normalize = false );

    declareProperty( "DoSystematicPlots", bDoSystematicPlots = false );
    declareProperty( "DoEfficiencyPlots", bDoEfficiencyPlots = false );

    declareProperty( "NPadX", m_npadX = 4);
    declareProperty( "NPadY", m_npadY = 3);
    declareProperty( "QCDFitHistogram", m_QCDfitHist="lepPt");
    declareProperty( "QCDCutValue", m_QCDcut=0);   
    declareProperty( "makeQCDFit", m_makeQCDFit=false);	
    declareProperty( "makePDFSystematics", m_makePDFSyst=false);
    declareProperty( "PDFSelector", m_selectonPDF);
    declareProperty( "calcXsec", m_calcXSection=false);
    //m_czsyst  = std::pair<double,double>(0.,0.);
    //
    nevt_old_cut = 0.;
}

//------------------------------------------------------
ZeeDPlotter::~ZeeDPlotter()
{
    // Destructor
    delete EffBinGridTop;
    delete EffBinGridBot;
    delete EffBinGrid;

}

//------------------------------------------------------
StatusCode ZeeDPlotter::initialize()
{
    return StatusCode::SUCCESS;
}

//------------------------------------------------------
StatusCode ZeeDPlotter::finalize()
{
    return StatusCode::SUCCESS;
}

//------------------------------------------------------
StatusCode ZeeDPlotter::execute()
{

    gStyle->SetOptStat("emr");

    // Create directory Plots if it does not exist
    TString mkDirCommand = TString("if [ ! -d Plots ]; then mkdir Plots; fi");
    gSystem->Exec(mkDirCommand);
    /*
       if(m_filenames.size() == 0 || m_dirnames.size() == 0) {
       Error("ZeeDPlotter::execute", "No files or directories were given!");
       return StatusCode::FAILURE;
       }
       */
    if (m_makeQCDFit){
        RooMsgService::instance().setStreamStatus(1,false);
        RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);
        RooMsgService::instance().setSilentMode(true); 
        RooMsgService::instance().addStream(WARNING,Topic(Tracing),OutputFile("rf506_error.log")) ;
        gSystem->Load("libRooFit");
    }
    string dirname="SystematicsTexFormat/";
    string dirname2="SystematicsDatFormat/";
    string dirnameQCD="QCDSystematics/";
    gSystem->mkdir(dirname.c_str(), true);
    gSystem->mkdir(dirname2.c_str(), true);
    gSystem->mkdir(dirnameQCD.c_str(), true);


    for (std::vector<string>::iterator bosonIt = m_selectonCZ.begin(); bosonIt!=m_selectonCZ.end(); ++bosonIt){

        m_czvalue = -1.;
        m_czstat  = 10.;
        m_czsystUp  = 10.;
        m_czsystDown = 10.;
        m_Az = -1.;
        m_AzErr = 10.;

        m_QCD=-1;
        m_QCDErr=10.;

        m_boson = (*bosonIt);
        m_nameBos = (*bosonIt);   

        string dirname3=dirname+m_nameBos;
        gSystem->mkdir(dirname3.c_str(), true);
        string dirname4=dirname2+m_nameBos;
        gSystem->mkdir(dirname4.c_str(), true);
        string dirnameQCD2 = dirnameQCD+m_nameBos;
        gSystem->mkdir(dirnameQCD2.c_str(), true);

        m_path = "Boson";

        std::size_t found;

        if ((found=m_boson.find("enu")) != std::string::npos)
            m_boson="W";
        else if ((found=m_boson.find("munu"))!= std::string::npos)
            m_boson="Wmu";
        else if ((found=m_boson.find("ee")) != std::string::npos)
            m_boson="ZCC";
        else if ((found=m_boson.find("mumu")) != std::string::npos)
            m_boson="Zmumu";
        if ((found=(*bosonIt).find("plus")) != std::string::npos)
            m_path = "Plus";
        else if ((found=(*bosonIt).find("min")) != std::string::npos)
            m_path = "Minus";
        m_cutselector="All/NoShift/"+m_boson;


        std::cout << std::endl << std::endl << std::endl;
        std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        std::cout << "Calculating values for " << m_nameBos << " analysis " << std::endl;
        std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

        if(m_mcname != ""){
            std::cout << std::endl << std::endl << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << " C systematics " << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            this->GetCZSyst();       
        }

        if (m_makePDFSyst && m_mcname != ""){
            std::cout << std::endl << std::endl << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << " PDF systematics " << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            this->GetASyst();
        }

        if (m_makeQCDFit && (m_boson == "Wmu" || m_boson == "W")){
            std::cout << std::endl << std::endl << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << " QCD background " << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            this->GetQCD();
        }

        if (m_calcXSection && m_mcname != "" ){
            std::cout << std::endl << std::endl << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << " Cross-section " << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
            this->GetXSection();
        }


    }
    /*    
          if ( m_mcname != "") {
          this->GetCZSyst();        
          }    

          if ( m_makeQCDFit ){
          this->GetQCD();
          }	

          if (m_makePDFSyst){
          this->GetASyst();
          }
          */
    RunCutFlow();

    // get the first file
    vector<string>::const_iterator litrf = m_filenames.begin();

    // Loop over directories
    vector<string>::const_iterator litrdir = m_dirnames.begin();
    /*
       for( ; litrdir != m_dirnames.end(); ++litrdir) {

       ZeeDHistPlotter* ThePlotter = new ZeeDHistPlotter( *litrf );

       ThePlotter->SetNPadsXY(m_npadX,m_npadY);

       ThePlotter->UseStats(true);

       vector<string>::const_iterator litr = m_filenames.begin();

    // loop over files
    for( ; litr != m_filenames.end(); ++litr) {
    ThePlotter->ReadArray( *litrdir, *litr );
    }

    TString psname = "Plots/";
    psname.Append(*litrdir).Append(".ps");
    // MakePlots crashes in 15.6.1 (on InitRoot)

    ThePlotter->SetLogScale(m_logscale);
    ThePlotter->SetNormalize(m_normalize);

    ThePlotter->MakePlots(psname);

    Info("ZeeDPlotter::execute", "Plots save in file named: %s", psname.Data());
    delete ThePlotter;
    }


    // try string parser:
    if (m_dataoperation != "") {

    ZeeDHistPlotter* ParserPlotter = new ZeeDHistPlotter();

    ParserPlotter->UseStats(true);
    ParserPlotter->SetNPadsXY(m_npadX,m_npadY);


    cout << " Prepare plots for " << m_dataoperation << "," << endl;
    cout << " Save them in        Plots/ParserPlots.ps" << endl;

    ZeeDHistParser* Parser = new ZeeDHistParser(m_dataoperation);
    ZeeDHistArray* Result = Parser->eval();
    ParserPlotter->AddArray( new ZeeDHistArray(*Result) );
    delete Result;

    for (unsigned int i = 0; i < m_mcoperation.size(); i++) {
    cout << m_mcoperation[i] << endl;
    ZeeDHistParser* Parser = new ZeeDHistParser(m_mcoperation[i]);
    ZeeDHistArray* Result = Parser->eval();
    ParserPlotter->AddArray( new ZeeDHistArray(*Result) );
    delete Result;
    }

    ParserPlotter->SetLogScale(m_logscale);
    ParserPlotter->SetNormalize(m_normalize);

    ParserPlotter->MakePlots("Plots/ParserPlots.ps");
    delete ParserPlotter;
    }


    if (bDoSystematicPlots) {
    // Systematics plots (default for first file only)
    ZeeDHistPlotter* EnerSysPlotter = new ZeeDHistPlotter( *litrf );
    EnerSysPlotter->ReadArray("Sys_ZBos");
    EnerSysPlotter->ReadArray("Sys_ZBos/ElecEnUp");
    EnerSysPlotter->ReadArray("Sys_ZBos/ElecEnDown");

    EnerSysPlotter->SetLogScale(m_logscale);
    EnerSysPlotter->SetNormalize(m_normalize);

    EnerSysPlotter->MakePlots("Plots/EnerSysHist.ps");
    delete EnerSysPlotter;
}

if (bDoEfficiencyPlots) {
    // loop over all efficiencies and make plots
    vector<string>::const_iterator litrdirname = m_dirnames.begin();

    for( ; litrdirname != m_dirnames.end(); ++litrdirname) {

        if(*litrdirname == "Efficiency") {
            // loop over files
            vector<string>::const_iterator litrname = m_filenames.begin();

            for( ; litrname != m_filenames.end(); ++litrname) {

                ZeeDHistPlotter* EffPlotter = new ZeeDHistPlotter( *litrname );
                EffPlotter->ReadEfficiencyArray( *litrdirname );

                TString effpsname = "Plots/";
                effpsname.Append(*litrdirname).Append(".ps");
                EffPlotter->MakePlots(effpsname);
                cout << " Efficiency plots save in file named: " << effpsname << endl;

                delete EffPlotter;
            }
        } // if
    }

    // calculate efficiencies?
    RunEfficiencies();

}

// Draw data/mc overlayed W control plots
if (bDrawOverlayPlotsW) {
    this->DrawOverlayPlotsW();
}
*/
return StatusCode::SUCCESS;
}

// ----------------------------------------------------
void ZeeDPlotter::DrawOverlayPlotsW()
{

    TString dirName("/W Analysis/ControlW");

    // W boson plotter
    ZeeDHistPlotter* WenuPlotter = new ZeeDHistPlotter();
    WenuPlotter->UseStats();

    cout << "-->> Open data file " << endl;
    TFile* dataFile = TFile::Open("DataI.root");
    ZeeDHistArray dataArray = ZeeDHistArray(dataFile, dirName);
    dataFile->Close();

    cout << "-->> Open MC file " << endl;
    TFile* mcFile = TFile::Open("Wenu.root");
    ZeeDHistArray mcArray = ZeeDHistArray(mcFile, dirName);
    mcFile->Close();

    // scale
    //    mcArray *= 0.75133;

    cout << "-->> Add arrays " << endl;
    WenuPlotter->AddArray( &dataArray );
    WenuPlotter->AddArray( &mcArray );

    // Styling
    dataArray.SetLineColor(1);
    dataArray.SetMarkerSize(0.6);

    mcArray.SetLineColor(kOrange - 3);
    mcArray.SetFillStyle(1001);
    mcArray.SetFillColor(kYellow - 9);

    // Legend
    TLegend* legend     = new TLegend(0.55, 0.79, 0.72, 0.92, NULL, "brNDC");
    const TH1* histData = static_cast<TH1*>( dataArray.GetHistArray()->UncheckedAt(0) );
    const TH1* histMC   = static_cast<TH1*>( mcArray.GetHistArray()->UncheckedAt(0) );

    legend->AddEntry(histData,  "Data", "p");
    legend->AddEntry(histMC,    "W #rightarrow e #nu",   "f");
    legend->SetTextSize(0.06);
    legend->SetFillColor(10);
    legend->SetFillStyle(1001);
    legend->SetBorderSize(0);
    WenuPlotter->AddLegend(legend);

    cout << "-->> Make plots " << endl;
    WenuPlotter->MakePlots("Plots/WenuPlots.ps");

    delete legend;
    delete WenuPlotter;
}

// ----------------------------------------------------
void ZeeDPlotter::RunEfficiencies()
{

    // --------------------------
    vector<string> files = vector<string>();

    TString effdir = "Text/";
    GetDir(effdir, files);

    if(files.size() == 0) {
        cout << " there are no efficiency files given !" << endl;
        gSystem->Exit(EXIT_FAILURE);
    }

    // efficiencies will be saves in one ps file
    TPostScript* PSFile = new TPostScript("EfficiencyPlots.ps", 111);
    TCanvas* can = new TCanvas("EffPlots", "EffPlots", 600, 600);
    can -> SetFillColor(10);

    TString effname;


    Int_t f = 0;

    for (unsigned int i = 0; i < files.size() / 2; i++) {

        EffBinGridTop = new ZeeDBinGrid();
        EffBinGridBot = new ZeeDBinGrid();
        EffBinGrid = new ZeeDBinGrid();
        ErrEffBinGrid = new ZeeDBinGrid();

        EffBinGrid -> ReadFromFile(effdir + files[f]);
        EffBinGrid -> Reset(1.0);

        ErrEffBinGrid -> ReadFromFile(effdir + files[f]);
        ErrEffBinGrid -> Reset(1.0);

        EffBinGridBot -> ReadFromFile(effdir + files[f]);
        EffBinGridTop -> ReadFromFile(effdir + files[f + 1]);

        // Get efficiency
        GetEff(EffBinGridTop, EffBinGridBot, EffBinGrid, ErrEffBinGrid);
        EffBinGrid -> Print();

        PSFile->NewPage();
        can->Divide(1, 1);
        can->cd(1);
        gPad->SetLogy();
        gPad->SetRightMargin(0.12);
        gPad->SetLeftMargin(0.12);

        effname = files[f];
        effname.Remove(effname.Index("."));

        EffBinGrid->SetTitleX("#eta");
        EffBinGrid->SetTitleY("p_{T}");
        EffBinGrid->SetName("Efficiency");
        EffBinGrid->WriteIntoFile(effdir + "Eff/" + effname + ".txt");

        ErrEffBinGrid->SetTitleX("#eta");
        ErrEffBinGrid->SetTitleY("p_{T}");
        ErrEffBinGrid->SetName("ErrorEfficiency");
        ErrEffBinGrid->WriteIntoFile("Text/Error/" + effname + ".txt");

        EffBinGrid->Draw("Efficiency Grid");

        TLatex* eff_name = new TLatex(0.35, 0.97, effname);
        eff_name -> SetNDC(kTRUE);
        eff_name -> SetTextSize(0.04);
        eff_name -> SetLineWidth(2);
        eff_name -> SetTextFont(62);
        eff_name -> Draw("same");

        can -> Update();
        f += 2;
    }

    delete can;
    delete PSFile;

    //delete EffBinGridTop;

}
// ----------------------------------------------------
void ZeeDPlotter::GetDir(TString dir, vector<string> &files)
{
    // read dir now
    DIR* dp;
    struct dirent* dirp;

    if((dp  = opendir(dir)) == NULL) {
        cout << "Error opening " << dir << endl;
        return;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if(strstr(dirp->d_name, "Top") || strstr(dirp->d_name, "Bot")) {
            files.push_back(string(dirp->d_name));
        }
    }

    closedir(dp);
}
// ----------------------------------------------------
void ZeeDPlotter::GetEff(ZeeDBinGrid* Top, ZeeDBinGrid* Bot, ZeeDBinGrid*& Eff, ZeeDBinGrid*& ErrEff )
{

    for(Int_t idx = 0; idx < Eff->GetNumOfCells(); ++idx ) {

        ZeeDGridCell* cellTop = Top->GetCell(idx);
        ZeeDGridCell* cellBot = Bot->GetCell(idx);

        if(cellBot->GetValue() != 0.0) { // protect nans
            Eff->Set(idx, (cellTop->GetValue() / cellBot->GetValue()) * 100);

            if(cellTop->GetValue() != 0.0) {

                double A = cellTop->GetValue();
                double B = cellBot->GetValue();
                double C = B - A;
                double Err = sqrt( A * A * C + C * C * A ) / (B * B);

                ErrEff->Set(idx, Err);
            }
        } else {
            Eff->Set(idx, 100.0);
            ErrEff->Set(idx, 0.0);
        }
    }


}
// ----------------------------------------------------
void ZeeDPlotter::RunCutFlow()
{
    if (m_cutflow.size() == 0) {
        return;
    }

    ofstream cutFlow;

    string nameReco="CutFlow/CutFlow_"+m_nameBos+"_RECO__.tex";
    string nameGen ="CutFlow/CutFlow_"+m_nameBos+"_GEN__.tex";
    /*if (bCutFlowReco){
      cutFlow.open(nameReco.c_str());
      }
      else{
      cutFlow.open(nameGen.c_str());
      }*/

    //-------------------------------

    //vector<string> m_filenames={"out.root", "out_"+m_nameBos+".root"};

    //TString sCut = "";
    /*cutFlow << "\\begin{tabular}{|c|c c c|c c c|}" << endl;
      cutFlow << "\\hline" << endl;
      cutFlow << "Analysis cut &";
      cutFlow << " $N^{data}_{events}$ & $\\epsilon^{data}_{abs}$ & $\\epsilon^{data}_{rel}$ & " ;
      cutFlow << " $N^{MC}_{events}$ & $\\epsilon^{MC}_{abs}$ & $\\epsilon^{MC}_{rel}$  \\" ;
      cutFlow<< std::endl << " \\hline" << endl;
      */
    /*
       vector<string>::const_iterator litr = m_filenames.begin();
       vector<string>::const_iterator itcut = m_cutflow.begin();
       for( ; litr != m_filenames.end(); ++litr) {

       string type="MC";
       if (*litr=="out.root"){
       type="DATA";
       }

       TFile* myFile = new TFile((*litr).c_str());
       TH1D*  

       for( ; itcut != m_cutflow.end(); ++itcut) {

       cout << "Cut flow for " << sCut << endl;


    //ZeeDCutHistManager* CutHM = new ZeeDCutHistManager(*litr, m_cutselector);
    double apass = CutHM->GetCutFlow(sCut);
    TH1D*  myHist = (TH1D*)myFile -> Get("Control/EventWeight");
    double Evt = 0;

    if ( myHist != NULL) {
    Evt = myHist->GetEntries();
    }
    cutFlow << setw(10) << " & " << apass << " & ";

    cout << " File " << *litr << " pass ===> " << apass ;
    if (TMath::Abs(nevt_old_cut) > 1.) {
    cout.precision(3);
    cout << "\t( " << apass/nevt_old_cut*100. << " %)";
    cout.precision(4);

    cutFlow << apass/nevt_old_cut*100. << " & ";
    }
    nevt_old_cut = apass;


    if ( Evt != 0 ) {
    cout << " / " << Evt;
    cout.precision(4);
    cout << " = " << (apass / Evt) * 100 << "%" ;
    cutFlow << apass/Evt*100.;
    }

    cout << "\n";

    delete CutHM;
    }
    cutFlow << " \\\\" << endl;
    }
    cutFlow << "\hline " << endl << " \\end{tabular} " << endl;
    */
}

void ZeeDPlotter::GetASyst()
{
    cout << "Start A systematics calculator ..." << endl;
    cout << "-->> Open MC file " << endl;
    string pathPDFs = "/afs/desy.de/user/k/kgasniko/dust/analysis/mcPDF/";
    string pathPDFEig = "/afs/desy.de/user/k/kgasniko/dust/analysis/mcEig/";
    string fullNameRec = "All/NoShift/" + m_boson +"/" + m_path +"/" + m_refHistogramRec ;
    string fullNameGen = "All/NoShift/" + m_boson +"/GenInfo/"+ m_refHistogramGen ;
    string fullNameGen13 = "All/NoShift/" + m_boson +"/GenInfo13/"+ m_refHistogramGen ;
    if (m_boson.find("ZCC") != std::string::npos){
        fullNameGen   = "All/NoShift/Zmumu/GenInfo/"+m_refHistogramGen;
        fullNameGen13 = "All/NoShift/Zmumu/GenInfo13/"+m_refHistogramGen;

    }
    std::cout << fullNameGen13 << std::endl;
    string fullNameGenHist = "All/NoShift/NoCuts/GenInfo/"+m_GenHist ;

    double Az = 1;
    std::map<std::string,double>  Apdf, Cpdf, ApdfErr, CpdfErr, Epdf, EpdfErr;

    std::vector<double> Cpdfvar;
    std::vector<double> Apdfvar;
    std::vector<double> Epdfvar;
    std::vector<double> CpdfvarErr;
    std::vector<double> ApdfvarErr;
    std::vector<double> EpdfvarErr;
    double Integral=0;
    int nEig=52;
    for (int i=0; i<nEig+1; i++){
        //ugly, but something
        ostringstream convert;
        convert << i;
        string ii =convert.str();
        string filename=pathPDFEig+"out_"+m_nameBos+"_"+ii+".root";
        TFile* mcFile = new TFile(filename.c_str());

        TH1* genH = (TH1*)mcFile->Get(fullNameGenHist.c_str()); 
        TH1* hgen = (TH1*)mcFile->Get(fullNameGen.c_str());
        TH1* hgen13 = (TH1*)mcFile->Get(fullNameGen13.c_str());
        TH1* hrec = (TH1*)mcFile->Get(fullNameRec.c_str());


        if (genH == NULL) {
            Error("ZeeDPlotter::GetASyst", "Can not find generator level historgam, exit");
            return;
        }
        if (hgen == NULL) {
            Error("ZeeDPlotter::GetASyst","Can not find central gen histogram, exit");
            return;                
        }
        if (hgen13 == NULL) {
            Error("ZeeDPlotter::GetASyst","Can not find central gen13 histogram, exit");
            return;                
        }

        double sumGen13 = hgen13->GetSumOfWeights();
        double sumGen = hgen->GetSumOfWeights();
        double sumGenHist = genH->Integral(0, genH->GetNbinsX()+1);
        double sumRec= hrec->GetSumOfWeights();
        if (i==0) Integral = genH->GetEntries();
        double A = sumGen/sumGenHist;
        double C = sumRec/sumGen;
        double E = sumGen/sumGen13;        
        Apdfvar.push_back(A);
        Cpdfvar.push_back(C);
        Epdfvar.push_back(E);
        ApdfvarErr.push_back(sqrt(A*(1-A)/Integral));
        CpdfvarErr.push_back(sqrt(C*(1-C)/Integral));
        EpdfvarErr.push_back(sqrt(E*(1-E)/Integral));
        mcFile->Close();


    }
    //AvarUp/=50;
    //AvarUp=TMath::Sqrt(AvarUp);
    const double SfForPDFUncert = 1/1.62;


    string filenameSyst = "SystematicsTexFormat/"+(m_nameBos)+"/PDFSystematicsEIG.tex";
    FILE* sysOut=fopen(filenameSyst.c_str(),"w");
    int nVect=(nEig-1)/2;
    double CCent=0, ACent=0, ECent=0, CCentStatErr, ACentStatErr, ECentStatErr;
    double dECz=0, uECz=0, dEAz=0, uEAz=0, dEEz=0, uEEz=0;
    for (int i=0; i<nVect; i++){
        if (i==0) {
            CCent =Cpdfvar[i];
            ACent = Apdfvar[i];
            ECent = Epdfvar[i];

            CCentStatErr =CpdfvarErr[i];
            ACentStatErr =ApdfvarErr[i];
            ECentStatErr =EpdfvarErr[i];

            cout << "\n\n\n" ;
            cout << "Central C factor for PDF = " <<CCent<<" +- "<<CCentStatErr<<" (stat.)"<<endl;
            cout << "Central A factor for PDF = " <<ACent<<" +- "<<ACentStatErr<<" (stat.)"<<endl;
            cout << "Central E factor for PDF = " <<ECent<<" +- "<<ECentStatErr<<" (stat.)"<<endl;

            printf("%20s %10.2s %10.2s %10.2s %10.2s %10.2s %10.2s \n", "Source", "Cz Up %", "Cz Down %", "Az Up %","Az Down %", "E Up %", "E Down %"); 
            cout << "------------------------------------------------------------------------------------" << endl;

            fprintf( sysOut,"%10s & %10s & %10s & %10s & %10s & %10s & %10s \\\\ \n","Eigenvector","$C_Z$ Up(\\%)","$C_Z$ Down(\\%)","$A_Z$ Up(\\%)","$A_Z$ Down(\\%)", "$E_Z$ Up (\\%)", "$E_Z$ Down (\\%)");
            fprintf( sysOut, "\\hline \n");
        }
        else {
            int iPos=i*2-1;
            int iNeg=i*2;
            double CPlus = Cpdfvar[iPos];
            double CMinus = Cpdfvar[iNeg];
            double APlus = Apdfvar[iPos];
            double AMinus = Apdfvar[iNeg];
            double EPlus = Epdfvar[iPos];
            double EMinus = Epdfvar[iNeg];

            double uCz = (CPlus-CCent)/CCent*100.;
            double dCz = (CMinus-CCent)/CCent*100.; 
            double uAz = (APlus-ACent)/ACent*100.;
            double dAz = (AMinus-ACent)/ACent*100.;
            double uEz = (EPlus-ECent)/ECent*100.;
            double dEz = (EMinus-ECent)/ECent*100.;

            // multiply by 1/1.62
            uCz = uCz * SfForPDFUncert;
            dCz = dCz * SfForPDFUncert; 
            uAz = uAz * SfForPDFUncert;
            dAz = dAz * SfForPDFUncert;
            uEz = uEz * SfForPDFUncert;
            dEz = dEz * SfForPDFUncert;



            fprintf( sysOut,"%5d &%14.3f &%14.3f &%14.3f &%14.3f &%14.3f &%14.3f\\\\ \n",i,uCz,dCz,uAz,dAz,uEz,dEz);

            printf("%5d %15s %10.2f %10.2f %10.2f %10.2f %10.2f %10.2f \n",i, "CT10nlo",
                    uCz,
                    dCz,
                    uAz,
                    dAz,
                    uEz,
                    dEz
                  );

            if ( (dCz < 0) && (uCz<0) ) {
                dECz +=  uCz<dCz ?  uCz*uCz : dCz*dCz;
            }
            else if ( (dCz<0) && ( uCz>0 ) ) {
                dECz += dCz*dCz;
                uECz += uCz*uCz;
            }
            else if ( (dCz>0) && ( uCz<0 ) ) {
                dECz += uCz*uCz;
                uECz += dCz*dCz;
            }
            else {
                uECz +=  uCz>dCz ?  uCz*uCz : dCz*dCz;
            }

            if ( (dAz < 0) && (uAz<0) ) {
                dEAz +=  uAz<dAz ?  uAz*uAz : dAz*dAz;
            }
            else if ( (dAz<0) && ( uAz>0 ) ) {
                dEAz += dAz*dAz;
                uEAz += uAz*uAz;
            }
            else if ( (dAz>0) && ( uAz<0 ) ) {
                dEAz += uAz*uAz;
                uEAz += dAz*dAz;
            }
            else {
                uEAz +=  uAz>dAz ?  uAz*uAz : dAz*dAz;
            }

            if ( (dEz < 0) && (uEz<0) ) {
                dEEz +=  uEz<dEz ?  uEz*uEz : dEz*dEz;
            }
            else if ( (dEz<0) && ( uEz>0 ) ) {
                dEEz += dEz*dEz;
                uEEz += uEz*uEz;
            }
            else if ( (dEz>0) && ( uEz<0 ) ) {
                dEEz += uEz*uEz;
                uEEz += dEz*dEz;
            }
            else {
                uEEz +=  uEz>dEz ?  uEz*uEz : dEz*dEz;
            }
        }
    }

    cout << "------------------------------------------------------------------------------------" <<endl;
    printf("%20s %10.3f %10.3f %10.3f %10.3f %10.3f %10.3f \n","Total",
            sqrt(uECz),
            -sqrt(dECz),
            sqrt(uEAz),
            -sqrt(dEAz),
            sqrt(uEEz),
            -sqrt(dEEz)
          );
    cout << "------------------------------------------------------------------------------------" <<endl;
    cout << "\n\n\n";

    fprintf( sysOut, "\\hline \n");
    fprintf( sysOut, "\\hline \n");
    fprintf( sysOut,"%10s & %10.3f & %10.3f & %10.3f & %10.3f  & %10.3f & %10.3f \\\\ \n","Total uncertainty",sqrt(uECz),-sqrt(dECz),sqrt(uEAz),-sqrt(dEAz), sqrt(uEEz),-sqrt(dEEz));
    fprintf( sysOut, "\\hline \n");
    fprintf( sysOut, "\\hline \n");
    fclose(sysOut);

    std::cout << "111" << std::endl;
    //double dTotUp   =  sqrt( pow(m_czsyst.first,  2) + uECz);
    //double dTotDown = -sqrt( pow(m_czsyst.second, 2) + dECz);
    double aTot=0, cTot=0, eTot=0;
    for (std::vector<string>::iterator pdfName = m_selectonPDF.begin(); pdfName!=m_selectonPDF.end(); ++pdfName) { 
        string filename= "out_"+(m_nameBos)+"_"+*pdfName+".root";
        filename=pathPDFs+filename;
        cout << filename << endl;

        TFile* mcFile = TFile::Open(filename.c_str());

        //double refRec = 0;
        double refGen = 1;    

        TH1* genH = (TH1*)mcFile->Get(fullNameGenHist.c_str()); 
        TH1* hgen = (TH1*)mcFile->Get(fullNameGen.c_str());
        TH1* hgen13 = (TH1*)mcFile->Get(fullNameGen13.c_str());
        TH1* hrec = (TH1*)mcFile->Get(fullNameRec.c_str());

        if (genH == NULL) {
            Error("ZeeDPlotter::GetASyst", "Can not find generator level historgam, exit");
            return;
        }
        if (hgen == NULL) {
            Error("ZeeDPlotter::GetASyst","Can not find central gen histogram, exit");
            return;                
        }
        if (hgen13 == NULL) {
            Error("ZeeDPlotter::GetASyst","Can not find central gen13 histogram, exit");
            return;                
        }
        if (hrec == NULL) {
            Error("ZeeDPlotter::GetASyst", "Can not find rec historgram, exit");
            return;
        }			

        double sumGen = hgen->GetSumOfWeights();
        double sumGen13 = hgen13->GetSumOfWeights();
        double sumGenHist = genH->Integral(0, genH->GetNbinsX()+1);
        std::cout << sumGen << "  " << sumGen13<<std::endl;
        double sumRec= hrec->GetSumOfWeights();
        double A = sumGen/sumGenHist;
        double C = sumRec/sumGen;
        double E = sumGen/sumGen13;       
        std::cout << E << std::endl; 
        mcFile->Close();
        refGen = sumGen ;
        Apdf[*pdfName]=A;
        Cpdf[*pdfName]=C;
        Epdf[*pdfName]=E;
        ApdfErr[*pdfName]=sqrt(A*(1-A)/Integral);
        CpdfErr[*pdfName]=sqrt(C*(1-C)/Integral);
        EpdfErr[*pdfName]=sqrt(E*(1-E)/Integral);
        aTot+=A;
        cTot+=C;
        eTot+=E;

    }
    int nPDFSets = Apdf.size();
    aTot=ACent;
    cTot=CCent;
    eTot=ECent;

    std::cout << "Acceptance = " << aTot << std::endl << std::endl;
    std::cout << "C  =  " << cTot<< std::endl << std::endl;
    std::cout << "E  =  " << eTot<< std::endl << std::endl;
    //printf("%55s %10s %10s \n", "Source", "%", "Total"); 
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;

    double dEE = 0;
    double uEE = 0;

    double dE = 0;
    double uE = 0;
    double dEC = 0;
    double uEC = 0;	
    int nUpA=0, nUpC=0, nUpE=0;
    string fileSyst2 = "SystematicsTexFormat/"+(m_nameBos)+"/PDFSystematicsSets.tex";
    FILE* sysOut2=fopen(fileSyst2.c_str(), "w");
    fprintf( sysOut2,"%10s & %10s & %10s & %10s & %10s \\\\ \n","PDF set","$C_Z$(\\%)","d$C_Z$(\\%)","$A_Z$(\\%)","d$A_Z$(\\%)", "$E_Z$(\\%)","d$E_Z$(\\%)");
    fprintf( sysOut2, "\\hline \n");

    string fileSyst3="SystematicsDatFormat/"+(m_nameBos)+"/PDFSystematics.txt";
    FILE* sysOut3=fopen(fileSyst3.c_str(), "w");

    fprintf (sysOut3, "A %0.3f \n", aTot);
    fprintf (sysOut3, "E %0.3f \n", eTot);

    printf("%55s %10.5s %10.5s %10.5s %10.5s", "PDFSet", "A", "dA", "C", "dC", "E", "dE");
    cout << endl;
    for (std::map<std::string,double>::iterator pairA = Apdf.begin(); pairA != Apdf.end(); ++ pairA){ 
        std::string name = (*pairA).first;
        cout << Apdf[name] << "  " << aTot << endl;
        double u = (Apdf[name]-aTot)/aTot*100.;
        double d = Apdf[name];
        double uC = (Cpdf[name]-cTot)/cTot*100.;
        double dC = Cpdf[name];
        double uEf = (Epdf[name]-eTot)/eTot*100.;
        double dEf = Epdf[name];

        //std::cout << " A = " << std::endl;	
        fprintf( sysOut2,"%10s & %14.3f & %14.3f & %14.3f & %14.3f & %14.3f & %14.3f \\\\ \n",name.c_str(),dC,uC,u,d, uEf, dEf);


        printf("%55s %10.5f %10.5f %10.5f %10.5f %10.5f %10.5f\\\\ \n",name.c_str(),
                d,
                u,
                dC,
                uC,
                dEf,
                uEf
              );

        /*if (u>0) {
          dE += u*u;
          nUpA++;
          }
          else {
          uE += u*u;
          }
          if (uC > 0){
          dEC += uC*uC;
          nUpC++;
          } else 
          uEC += uC*uC;

          if (uEf > 0){
          dEE += uEf*uEf;
          nUpE++;
          } else 
          uEE += uEf*uEf;
          */
        if (u*u > dE){
            dE=u*u;
        }
        if (uC*uC > dEC){
            dEC=uC*uC;
        }
        if (uEf*uEf > dEE){
            dEE=uEf*uEf;
        }
    }

    //PS uncertainties
    string fileName="";
    if (m_nameBos.find("enu") != std::string::npos ){
        fileName="out_Wenu.root";
    }
    else if (m_nameBos.find("munu") != std::string::npos ){
        fileName="out_Wmunu.root";
    }
    double dAPS=0.9, dEPS=0, dCPS=0;
    if (m_nameBos.find("nu") != std::string::npos){
        fullNameGen = "All/NoShift/" + m_boson +"/GenInfo/"+ m_nameBos ;
        fullNameGen13 = "All/NoShift/" + m_boson +"/GenInfo13/"+ m_nameBos;
        fullNameGenHist = "All/NoShift/NoCuts/GenInfo/"+m_nameBos ;

        TFile* mcFile = new TFile(fileName.c_str());
        TH1* genH = (TH1*)mcFile->Get(fullNameGenHist.c_str()); 
        TH1* hgen = (TH1*)mcFile->Get(fullNameGen.c_str());
        TH1* hgen13 = (TH1*)mcFile->Get(fullNameGen13.c_str());
        TH1* hrec = (TH1*)mcFile->Get(fullNameRec.c_str());

        if (genH == NULL) {
            Error("ZeeDPlotter::GetASyst", "Can not find generator level historgam, exit");
            return;
        }
        if (hgen == NULL) {
            Error("ZeeDPlotter::GetASyst","Can not find central gen histogram, exit");
            return;                
        }
        if (hgen13 == NULL) {
            Error("ZeeDPlotter::GetASyst","Can not find central gen13 histogram, exit");
            return;                
        }
        if (hrec == NULL) {
            Error("ZeeDPlotter::GetASyst", "Can not find rec historgram, exit");
            return;
        }			

        double sumGen = hgen->Integral(0, -1);
        double sumGen13 = hgen13->Integral(0, -1);
        double sumGenHist = genH->Integral(0, -1);
        double sumRec= hrec->GetSumOfWeights();
        double A = sumGen/sumGenHist;
        double C = sumRec/sumGen;
        double E = sumGen/sumGen13;        
        dAPS=(A-aTot)/aTot*100.;
        dEPS=(E-eTot)/eTot*100.;
        dCPS=(C-cTot)/cTot*100.;
        mcFile->Close();
        //End of PS uncertainty calculation
    }
    cout << "------------------------------------------------------------------------------------" << endl;
    printf ("%55s %10.5f \n"," A PS",
            dAPS);
    printf ("%55s %10.5f \n"," E PS",
            dEPS);
    printf ("%55s %10.5f \n"," C PS",
            dCPS);
    /*
       dE=dE/nUpA;
       uE=uE/(nPDFSets-nUpA);
       dEC = dEC/nUpC;
       uEC = uEC/(nPDFSets-nUpC);
       dEE = dEE/nUpE;
       uEE = uEE/(nPDFSets-nUpE);
       */
    cout << "------------------------------------------------------------------------------------" << endl;

    printf("%55s %10.5f %10.5f\n","Total A",
            sqrt(dE), -sqrt(dE)
          );
    printf("%55s %10.5f %10.5f\n","Total C",
            sqrt(dEC), -sqrt(dEC)
          );

    printf("%55s %10.5f %10.5f\n","Total E",
            sqrt(dEE), -sqrt(dEE)
          );
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "\n\n\n";

    fprintf( sysOut2, "\\hline \n");
    fprintf( sysOut2, "\\hline \n");
    fprintf( sysOut2,"%10s & %10.3f & %10.3f & %10.3f & %10.3f  & %10.3f & %10.3f \\\\ \n","Total uncertainty",sqrt(dEC),-sqrt(dEC),sqrt(dE),-sqrt(uE), sqrt(dEE), -sqrt(uEE));
    fprintf( sysOut2, "\\hline \n");
    fprintf( sysOut2, "\\hline \n");
    fclose(sysOut2);

    fprintf(sysOut3, "A PDFset %0.3f \n", dE);
    fprintf(sysOut3, "E PDFset %0.3f \n", dEE);
    fprintf(sysOut3, "A PS %0.3f \n", dAPS);
    fprintf(sysOut3, "E PS %0.3f \n", dEPS); 
    fclose(sysOut3);

    dE=sqrt(dE*dE+uEAz*uEAz+dAPS*dAPS);
    uE=sqrt(dE*dE+dEAz*dEAz+dAPS*dAPS);
    uEC=sqrt(uEC*uEC+dECz*dECz);
    dEC=sqrt(uECz*uECz+dEC*dEC);
    uEE=sqrt(uEEz*uEEz+dEE*dEE+dEPS*dEPS);
    dEE=sqrt(dEEz*dEEz+dEE*dEE+dEPS*dEPS);


    ofstream SystTableOut;
    ofstream SystForDatOut;
    ofstream SystUncertOut;
    string SystTableName = "SystematicsTexFormat/"+m_nameBos+"/SystTable.tex";
    string SystForDataName = "SystematicsDatFormat/"+m_nameBos+"/C_syst.dat";
    string SystIncertName = "SystematicsTexFormat/"+m_nameBos+"/SystValues.tex";	

    SystTableOut.open(SystTableName.c_str(), std::ios::app);
    SystForDatOut.open(SystForDataName.c_str(), std::ios::app);
    SystUncertOut.open(SystIncertName.c_str(), std::ios::app);

    double dTotUp   =  sqrt( pow(m_czsystUp,  2) + uEC);
    double dTotDown = -sqrt( pow(m_czsystDown, 2) + dEC);


    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "PDFEigUp"<<"}{"<<fixed<<setprecision(2)<<sqrt(uECz)<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "PDFEigDown"<<"}{"<<fixed<<setprecision(2)<<-sqrt(dECz)<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "PDFSetUp"<<"}{"<<fixed<<setprecision(2)<<sqrt(dEC)<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "PDFSetDown"<<"}{"<<fixed<<setprecision(2)<<-sqrt(uEC)<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "TOTUp"<<"}{"<<fixed<<setprecision(2)<<dTotUp<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "TOTDown"<<"}{"<<fixed<<setprecision(2)<<dTotDown<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "AEigUp"<<"}{"<<fixed<<setprecision(2)<<sqrt(uEAz)<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "AEigDown" <<"}{"<<fixed<<setprecision(2)<<-sqrt(dEAz)<<"}"<<endl; 
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "APDFUp"<<"}{"<<fixed<<setprecision(2)<<sqrt(dE)<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "APDFDown" <<"}{"<<fixed<<setprecision(2)<<-sqrt(uE)<<"}"<<endl; 
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "EEigUp"<<"}{"<<fixed<<setprecision(2)<<sqrt(uEEz)<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "EEigDown" <<"}{"<<fixed<<setprecision(2)<<-sqrt(dEEz)<<"}"<<endl; 
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "EPDFUp"<<"}{"<<fixed<<setprecision(2)<<sqrt(dEE)<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "EPDFDown" <<"}{"<<fixed<<setprecision(2)<<-sqrt(uEE)<<"}"<<endl; 
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "AEig"<<"}{"<<fixed<<setprecision(2)<<(sqrt(uEAz)+sqrt(dEAz))/2<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "APDF"<<"}{"<<fixed<<setprecision(2)<<(sqrt(dE)+sqrt(uE))/2<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "EEig"<<"}{"<<fixed<<setprecision(2)<<(sqrt(uEEz)+sqrt(dEEz))/2<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "EPDF"<<"}{"<<fixed<<setprecision(2)<<(sqrt(dEE)+sqrt(uEE))/2<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "A"<<"}{"<<fixed<<setprecision(2)<<aTot<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "E"<<"}{"<<fixed<<setprecision(2)<<eTot<<"}"<<endl;

    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "APS"<<"}{"<<fixed<<setprecision(2)<<dAPS<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "EPS"<<"}{"<<fixed<<setprecision(2)<<dEPS<<"}"<<endl;
    SystUncertOut<<"\\newcommand{\\"<<m_nameBos << "CPS"<<"}{"<<fixed<<setprecision(2)<<dCPS<<"}"<<endl;

    //        SystTableOut<<"Pileup  &  "<<fixed<<setprecision(2)<<m_pileup_u<<"  &  "<<m_pileup_d<<"  \\\\"<<endl;
    //      SystTableOut<<"Opposite charge requirement  &  "<<fixed<<setprecision(2)<<-charge_miss_id<<"  &  "<<charge_miss_id<<"  \\\\"<<endl;
    SystTableOut<<"PDF  &  "<<fixed<<setprecision(2)<<sqrt(uEC)<<"  &  "<<-sqrt(dEC)<<"  \\\\"<<endl;              
    SystTableOut<<"\\hline"<<endl;        
    SystTableOut<<"Total  &  "<<fixed<<setprecision(2)<<dTotUp<<"  &  "<<dTotDown<<"  \\\\"<<endl;        



    //     SystForDatOut<<"PILEUP_W__1    "<<fixed<<setprecision(2)<<m_pileup_u<<"    "<<m_pileup_d<<endl;
    //     SystForDatOut<<"ChargeIDZ    "<<fixed<<setprecision(2)<<-charge_miss_id<<"    "<<charge_miss_id<<endl;
    SystForDatOut<<"PDF    "<<fixed<<setprecision(2)<<sqrt(uEC)<<"    "<<-sqrt(dEC)<<endl;  


    SystUncertOut.close();
    SystForDatOut.close();
    SystTableOut.close(); 

    m_Az=ACent;
    m_AzErr=(uE+dE)/2;
    m_Ez=ECent;
    m_EzErr=(uEE+dEE)/2;


}

void ZeeDPlotter::GetCZSyst() 
{
    // CZ is the ratio of N data to N MC events for the BosonY distribution.
    cout << "Start CZ calculator ..." << endl;
    cout << "-->> Open MC file " << endl;

    string filename= "out_"+(m_nameBos)+".root";
    cout << filename << endl;
    TFile* mcFile = TFile::Open(filename.c_str());

    double refRec = 0;
    double refGen = 1;    
    double cZcent = 1;
    double cZcentStat = 0.;
    double Az = 1;

    // also for up/down variations
    std::map<std::string,double>  recUp;
    std::map<std::string,double>  recDown;
    std::map<std::string,double>  genUp;
    std::map<std::string,double>  genDown;

    std::map<std::string,double>  czUp;
    std::map<std::string,double>  czDown;
    std::map<std::string, double> nToyUp;
    std::map<std::string, double> nToyDown;

    vector<string> subNames;
    TH1* hgenCent = NULL;


    for (std::vector<string>::iterator syst=m_systlist.begin(); syst!=m_systlist.end();++syst){
        string fullNameRec = "All/" + *syst + "/" + m_boson +"/" + m_path +"/" + m_refHistogramRec ;
        string fullNameGen = "All/" + *syst + "/" + m_boson +"/GenInfo/"+ m_refHistogramGen ;
        if (m_boson.find("ZCC") != std::string::npos){
            fullNameGen = "All/NoShift/Zmumu/GenInfo/"+m_refHistogramGen;
        }

        //std::cout << "fullNameRec = " << fullNameRec << " fullNameGen = " << fullNameGen << std::endl;
        if ( *syst == "NoShift")  {
            // Reference
            string fullNameGenHist = "All/NoShift/NoCuts/GenInfo/"+m_GenHist ;
            TH1* genH = (TH1*)mcFile->Get(fullNameGenHist.c_str()); 
            TH1* hrec = (TH1*)mcFile->Get(fullNameRec.c_str());
            TH1* hgen = (TH1*)mcFile->Get(fullNameGen.c_str());

            hgenCent = hgen;

            if (genH == NULL) {
                Error("ZeeDPlotter::GetCZSyst", "Can not find generator level historgam, exit");
                return;
            }


            if (hrec == NULL) {
                Error("ZeeDPlotter::GetCZSyst","Can not find central reco histogram, exit");
                return;                
            }            
            if (hgen == NULL) {
                Error("ZeeDPlotter::GetCZSyst","Can not find central gen histogram, exit");
                return;                
            }            


            double sumRec = hrec->GetSumOfWeights();
            double sumGen = hgen->GetSumOfWeights();
            double sumGenHist = genH->GetSumOfWeights();
            double err = 0;

            if (hrec->IntegralAndError(0,1000,err)>0) {
            }


            //            std::cout << aa << " " << sumRec << " " << err <<  " hhhah" << std::endl;



            refRec = sumRec ;
            refGen = sumGen ;
            cZcent = refRec/refGen;
            cZcentStat = err/sumGen*100.; 
            Az = sumGen/sumGenHist;
            //std::cout << " sumRec, Sum Gen = " << sumRec << " "<< sumGen << std::endl;
            //std::cout << " C_Z = " << cZcent << std::endl;

        } else if ((*syst).rfind("ToyMC") != std::string::npos) { // Toy systematics

            string fullNameRec = "All/" + *syst + "/" + m_boson +"/" + m_path +"/" + m_refHistogramRecToy;
            string fullNameGen = "All/" + *syst + "/" + m_boson +"/GenInfo/"+ m_refHistogramGenToy ;
            if (m_boson.find("ZCC") != std::string::npos){
                fullNameGen = "All/NoShift/Zmumu/GenInfo/"+m_refHistogramGenToy;
            }

            //cout  << fullNameRec << "  " << fullNameGen << endl;
            string subname = (*syst).substr(0, (*syst).rfind("MC"));
            //cout << subname << endl;
            TH1* hrec = (TH1*)mcFile->Get(fullNameRec.c_str());
            TH1* hgen = (TH1*)mcFile->Get(fullNameGen.c_str()); 

            if (hrec == NULL) {
                Error("ZeeDPlotter::GetCZSyst","Can not find data histogram for systematics %s, exit",(*syst).c_str());
                return;                
            }            

            if (hgen == NULL ) {
                hgen = hgenCent;
                Warning("ZeeDPlotter:GetCZSyst","Could not find gen info for systematic variation %s, use central",(*syst).c_str());
            }

            double sumRec = hrec->GetSumOfWeights();
            double sumGen = hgen->GetSumOfWeights();

            //std::cout << subname << std::endl;
            //std::cout << " sumRec, Sum Gen = " << sumRec << " "<< sumGen << std::endl;
            double cZ = sumRec/sumGen;
            //std::cout <<  " cZ = " << cZ << std::endl;
            //if (cZ > cZcent){
            if (czUp.find(subname) == czUp.end()){
                //std::cout << 1 << std::endl;
                czUp[subname]=cZ*cZ;
                czDown[subname]=cZ;
                //czUp[subname]=(cZ-cZcent)*(cZ-cZcent);
                nToyUp[subname]=1;
            }else {
                //std::cout << 2 << std::endl;
                czUp[subname]+=cZ*cZ;
                czDown[subname]+=cZ;
                nToyUp[subname]+=1;
            }
            /* } else {
            //std::cout << subname << " 22 " << std::endl;
            if (czDown.find(subname) != czDown.end()){
            czDown[subname]=(cZ-cZcent)*(cZ-cZcent);
            nToyDown[subname]=1;
            }else {                    
            czDown[subname]+=(cZ-cZcent)*(cZ-cZcent);
            nToyDown[subname]+=1;
            }*/
            // }

        }else { //Normal systematics

            // Determine if it is up or donw variation.
            bool Up = ( (*syst).rfind("Up") == (*syst).length()-2);
            bool Down = ( (*syst).rfind("Down") == (*syst).length()-4);           

            string subname = (*syst);
            if (Up || Down) {
                subname = Up ? (*syst).substr(0,(*syst).rfind("Up")) : (*syst).substr(0,(*syst).rfind("Down"));
            }

            //std::cout << "Subname: " << subname << std::endl; 
            TH1* hrec = (TH1*)mcFile->Get(fullNameRec.c_str());
            TH1* hgen = (TH1*)mcFile->Get(fullNameGen.c_str());
            if (hrec == NULL) {
                Error("ZeeDPlotter::GetCZSyst","Can not find data histogram for systematics %s, exit",(*syst).c_str());
                return;                
            }            

            if (hgen == NULL ) {
                hgen = hgenCent;
                Warning("ZeeDPlotter:GetCZSyst","Could not find gen info for systematic variation %s, use central",(*syst).c_str());
            }

            double sumRec = hrec->GetSumOfWeights();
            double sumGen = hgen->Integral(0, -1);

            //std::cout << " sumRec, Sum Gen = " << sumRec << " "<< sumGen << std::endl;
            double cZ = sumRec/sumGen;

            //std::cout << " cZ = " << cZ << std::endl;
            if ( Up  || !Down ) {
                recUp[subname] = sumRec;
                genUp[subname] = sumGen;
                czUp[subname] = cZ;
            } else if ( Down ) {
                recDown[subname] = sumRec;
                genDown[subname] = sumGen;
                czDown[subname] = cZ;
            }      


        }        
    }

    mcFile->Close();
    std::cout << "\n\n\n" ;

    std::cout << "Central C factor = " << cZcent << std::endl << std::endl;

    ofstream  SystTableOut;
    ofstream  SystUncertOut;
    ofstream  SystForDatOut;

    string systTableOutName = "SystematicsTexFormat/"+m_nameBos+"/SystTable.tex";
    string systUncertOutName = "SystematicsTexFormat/"+m_nameBos+"/SystValues.tex";
    string systForDatOutName = "SystematicsDatFormat/"+m_nameBos+"/C_syst.dat";
    SystTableOut.open(systTableOutName.c_str());
    SystUncertOut.open(systUncertOutName.c_str());
    SystForDatOut.open(systForDatOutName.c_str());

    SystTableOut<<"Statistics  &  "<<fixed<<setprecision(2)<<cZcentStat<<"  &  "<<-cZcentStat<<"  \\\\"<<endl;    


    m_czvalue=cZcent;
    printf("%55s %10s %10s \n", "Source", "Up %", "Down %"); 
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;

    printf("%55s %10.5f %10.5f \n","Statistics", cZcentStat, -cZcentStat);    
    m_czstat=cZcentStat;
    double dE = 0;
    double uE = 0; 
    double elecEnScaleTotal=0.0;
    double muonResolutionTotal=0.0;   
    for (std::map<std::string,double>::iterator pairRec = czUp.begin(); pairRec != czUp.end(); ++ pairRec){ 
        string name = (*pairRec).first;

        double u = (czUp[name]-cZcent)/cZcent*100.;
        double d = -u;
        if (czDown.find(name) != czDown.end()){
            d = (czDown[name]-cZcent)/cZcent*100.;
        }

        //std::cout << name << std::endl;
        if (name.rfind("Toy")!=std::string::npos){ // Toy systematics
            double centr = czDown[name]/nToyUp[name];
            double rms = czUp[name]-2*czDown[name]*centr+centr*centr*nToyUp[name];
            if (rms < 1e-10 && rms > -1e-10){
                rms = 0;
            }
            //std::cout << rms << std::endl;
            rms=sqrt(rms/(nToyUp[name]-1));
            //u = centr+rms-cZcent;
            //std::cout << centr<< " " << rms << " " << nToyUp[name] <<std::endl;
            //d = centr-rms-cZcent;
            u=rms/centr*100.;
            d=-rms/centr*100.;
            /*
               u = sqrt(czUp[name]/nToyUp[name]);
               d = sqrt(czDown[name]/nToyDown[name]);
               std::cout << u << "   " << d << std::endl*/;
        }

        double ddd = fabs((u-d)/2);

        if (name.rfind("ElecEn")!=std::string::npos){
            elecEnScaleTotal+=ddd*ddd;
            if (name.rfind("R12") !=std::string::npos){
                name="ElecEnRStat"; 
            }  
        }else if (name.rfind("MuSmearing")!=std::string::npos){
            muonResolutionTotal+=ddd*ddd;
        }

        if (u !=0 && d!=0){
            printf("%55s %10.5f %10.5f \n",name.c_str(),
                    u,
                    d
                  );

            if ( (d < 0) && (u<0) ) {
                dE +=  u<d ?  u*u : d*d;
            }

            else if ( (d<0) && ( u>0 ) ) {
                dE += d*d;
                uE += u*u;
            }
            else if ( (d>0) && ( u<0 ) ) {
                dE += u*u;
                uE += d*d;
            }
            else {
                uE +=  u>d ?  u*u : d*d;
            }
            int pres=2;
            if (u<0.01 && u>0.001){
                pres=3;
            }
            SystTableOut<< name <<"  &  "<<fixed<<setprecision(pres)<<u<<"  &  "<<d<<"  \\\\"<<endl;

            SystForDatOut<<name<<"    "<<fixed<<setprecision(pres)<<u<<"    "<<d<<endl;
            SystUncertOut<<"\\newcommand{\\"<<name + "U" + m_nameBos<<"}{"<<fixed<<setprecision(pres)<<u<<"}"<<endl;
            SystUncertOut<<"\\newcommand{\\"<<name + "D" + m_nameBos<<"}{"<<fixed<<setprecision(pres)<<d<<"}"<<endl;
            SystUncertOut<<"\\newcommand{\\"<<name + m_nameBos << "}{"<<fixed<<setprecision(pres)<< ddd << "}"<<endl;
        }


    }
    double smearErr=0.0;
    if (m_boson.find("W") != std::string::npos){
        if (strcmp(m_path.c_str(),"Plus") == 0){
            if (strcmp(m_boson.c_str(), "W") == 0 )
                smearErr=0.2;
            if (strcmp(m_boson.c_str(), "Wmu") == 0)
                smearErr=0.16;
        }else if (strcmp(m_path.c_str(),"Minus") ==0){
            if (strcmp(m_boson.c_str(), "W") == 0 )
                smearErr=0.11;
            if (strcmp(m_boson.c_str(), "Wmu") == 0)
                smearErr=0.12; 
        }
    }
    if (smearErr>0){
        SystTableOut<< "HadrRecoilSmear  &  "<<fixed<<setprecision(2)<<smearErr<<"  &  "<<smearErr<<"  \\\\"<<endl;
        SystForDatOut<<"HadrRecoilSmear    "<<fixed<<setprecision(2)<<smearErr<<"    "<<smearErr<<endl;
        SystUncertOut<<"\\newcommand{\\HadrRecoilSmear"+m_nameBos<<"}{"<<fixed<<setprecision(2)<<smearErr << "}" <<endl;
        uE+=smearErr*smearErr;
        dE+=smearErr*smearErr; 
    }
    elecEnScaleTotal=sqrt(elecEnScaleTotal);
    muonResolutionTotal=sqrt(muonResolutionTotal);
    if (elecEnScaleTotal>0.0){
        SystUncertOut<<"\\newcommand{\\"<<"elecEnScaleTotal" + m_nameBos<<"}{"<<fixed<<setprecision(2)<<elecEnScaleTotal<<"}"<<endl;
    }
    if (muonResolutionTotal>0.0){
        SystUncertOut<<"\\newcommand{\\"<<"muonResolutionTotal"+m_nameBos<<"}{"<<fixed<<setprecision(2)<<muonResolutionTotal<<"}"<<endl;
    }

    SystTableOut<< "MCStat  &  "<<fixed<<setprecision(2)<< m_czstat <<"  &  "<< m_czstat <<"  \\\\"<<endl;
    SystForDatOut<<"MCStat    "<<fixed<<setprecision(2)<< m_czstat<<"    "<< m_czstat <<endl;
    SystUncertOut<<"\\newcommand{\\MCStat"+m_nameBos<<"}{"<<fixed<<setprecision(2)<< m_czstat << "}" <<endl;


    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    printf("%55s %10.5f %10.5f \n","Total",
            sqrt(uE+m_czstat*m_czstat),
            -sqrt(dE+m_czstat*m_czstat)
          );
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;

    std::cout << "\n\n\n";
    m_czsystUp=sqrt(uE+m_czstat*m_czstat);	
    m_czsystDown=-sqrt(dE+m_czstat*m_czstat);

    SystUncertOut.close();
    SystTableOut.close();
    SystForDatOut.close();


}


void ZeeDPlotter::MakeQCDFile(string boson, string path, double eventNum, string fold = "")
{
    TFile* dataFile = TFile::Open(m_dataname.c_str());

    if ( dataFile == NULL) {
        std::cout << " Failed to open data file, stop QCD calculation \n";
        return;
    }

    string plotsPath;
    string outFileName="";
    string procFileName=boson+"QCDName.txt";

    if (fold == "") {
        //FIXME
        outFileName ="";  
        plotsPath = "All/NoShift/"+boson+"/QCD/"+m_QCDfitHist+"/QCDTempl/"+path;
    } else {
        plotsPath="All/NoShift/"+boson+"/QCD/"+m_QCDfitHist+"/QCDVar/"+fold+"/"+path;
    }

    string fullNameGen = "All/NoShift/NoCuts/GenInfo/ZPosVtx";

    TDirectoryFile* templDir = (TDirectoryFile*)dataFile->Get(plotsPath.c_str());
    TList* histList = templDir->GetListOfKeys();
    vector< pair<TH1D*,string> > nameHist;
    int id=0;
    templDir->cd();
    while (histList->At(id) != NULL) {

        TKey* histKey = (TKey*)histList->At(id);
        TH1* histObj = (TH1*)histKey->ReadObj();	
        id++;

        const char* className = histObj->ClassName();
        string histName = string(histObj->GetName());
        unsigned long int bHist = histName.find(m_QCDfitHist.c_str());
        if (bHist == std::string::npos)
            continue;


        if (strcmp(className, "TH2D") == 0){

            TH2D* hist = (TH2D*)histObj;
            TH1D* dataHist =(TH1D*)malloc(sizeof(TH1D));

            string pathToHistogram = plotsPath+"/"+histObj->GetName();
            string nameNew = histName.erase(bHist, sizeof(m_QCDfitHist.c_str())-2);

            if (bHist == 0) {
                dataHist = hist->ProjectionY(nameNew.c_str(), m_QCDcutBin+1);
            }else{
                dataHist = hist->ProjectionX(nameNew.c_str(), m_QCDcutBin+1);	
            }

            nameHist.push_back(make_pair(new TH1D(*dataHist), pathToHistogram));

        }else if (strcmp(m_QCDfitHist.c_str(), histName.c_str()) == 0){
            TH1D* hist = (TH1D*) histObj;

            string pathToHistogram = plotsPath+"/"+histObj->GetName();
            EraseBins(hist, m_QCDcutBin+1);

            nameHist.push_back(make_pair(new TH1D(*hist), pathToHistogram));
        }
    }

    ifstream bkgList;
    bkgList.open(procFileName.c_str());
    bkgList.seekg(0, ios::beg);
    string procName;
    double xSec, err;
    bkgList >> procName >> xSec >> err;


    int vecSize = nameHist.size();	
    while (bkgList.good()){

        //reading process name, cross-section and error
        string filename = "out_"+procName+".root";
        TFile* bkgFile = TFile::Open(filename.c_str());
        double totGen = ((TH1D*)bkgFile->Get(fullNameGen.c_str()))->GetSumOfWeights();
        double scale = xSec*m_lumi/totGen;

        for (int i=0; i<vecSize; i++){

            string pathToHistogram=nameHist[i].second;
            const char* className = bkgFile->Get(pathToHistogram.c_str())->ClassName();

            if ( strcmp(className, "TH2D") == 0 ){
                TH2D* hist = (TH2D*)bkgFile->Get(pathToHistogram.c_str());
                string histName = string(hist->GetName());
                TH1D* newMcHist = (TH1D*)malloc(sizeof(TH1D));
                unsigned long int bHist = histName.find(m_QCDfitHist.c_str());
                string nameNew = histName.erase(bHist, bHist+sizeof(m_QCDfitHist.c_str())-2)+procName;
                if (bHist == 0){
                    newMcHist=hist->ProjectionY(nameNew.c_str(), m_QCDcutBin+1);
                }else 
                    newMcHist=hist->ProjectionX(nameNew.c_str(), m_QCDcutBin+1);
                nameHist[i].first->Add(newMcHist, -1*scale);
                hist->Delete();
                newMcHist->Delete();
            } else {
                TH1D* hist= (TH1D*)bkgFile->Get(pathToHistogram.c_str());
                EraseBins(hist, m_QCDcutBin+1);
                nameHist[i].first->Add(hist, -1*scale);
                hist->Delete();
            }
        }

        bkgList >> procName >> xSec>> err;
        bkgFile->Close();
    }
    string fileOutName="out_QCD"+boson+m_path+".root";
    TFile* fileOut = new TFile (fileOutName.c_str(), "RECREATE");
    string outPath = "All/NoShift/"+boson+"/"+m_path+"/";
    TDirectory* dirOut = fileOut->mkdir(outPath.c_str());
    fileOut->cd(outPath.c_str());

    for (int i=0; i<vecSize; i++){
        EraseBins(nameHist[i].first, -1);
        Double_t numHistQcd= nameHist[i].first->Integral(0,nameHist[i].first->GetNbinsX()+1); 
        nameHist[i].first->Scale(eventNum/numHistQcd);
        nameHist[i].first->Draw();
        nameHist[i].first->Write();
    }

    dirOut->Write();
    fileOut->Write();
    dataFile->Close();
    fileOut->Close();

}

void ZeeDPlotter::EraseBins(TH1D*& hist, int cut, bool inv){


    for (int i=0; i<=hist->GetNbinsX()+1; i++){
        bool cleanBin = inv?(i>=cut)&&(i!=-1):i<cut;
        if (hist->GetBinContent(i) < 0 || cleanBin) {
            hist->SetBinContent(i, 0.0);
            hist->SetBinError(i, 0.0);
        }
    }
}

void ZeeDPlotter::ScaleHist(TH1D*& hist, TH1D* totHist, double xSec){
    double scale=0.0;	
    if (totHist->GetSumOfWeights() != 0){ 
        scale = xSec*m_lumi/totHist->GetSumOfWeights();
    }
    hist->Scale(scale);
}


void ZeeDPlotter::GetQCD()
{


    TFile* dataFile = TFile::Open(m_dataname.c_str());
    if ( dataFile == NULL) {
        std::cout << " Failed to open data file, stop QCD calculation \n";
        return;
    }

    std::map<std::string,double>  qcdUp;
    std::map<std::string,double>  qcdDown;

    double qcdNumCentr=0;
    double qcdNumCentr2=0;
    double qcdErrFit = 1;
    double qcdErrFit2 = 1;
    double qcdErrStat = 1;

    TFile* fitResults = new TFile(("FitResults"+m_boson+m_path+".root").c_str(), "RECREATE");
    TDirectory* topDir = fitResults->mkdir("All");


    //pathsName
    string fullNameRecFitNoShift="All/NoShift/"+m_boson+"/QCD/"+m_QCDfitHist+"/QCDFit/"+m_path+"/"+m_QCDfitHist;
    string fullNameRecFitNoShiftComb="All/NoShift/"+m_boson+"/QCD/"+m_QCDfitHist+"/QCDFit/Boson/"+m_QCDfitHist;

    string fullNameRecTemplNoShift="All/NoShift/"+m_boson+"/QCD/"+m_QCDfitHist+"/QCDTempl/Boson/"+m_QCDfitHist;
    string fullNameRecTemplNoShift2="All/NoShift/"+m_boson+"/QCD/"+m_QCDfitHist+"/QCDVar/Boson/TrIso/"+m_QCDfitHist;
    if (m_boson == "Wmu"){
        //fullNameRecTemplNoShift = "All/NoShift/"+m_boson+"/QCD/"+m_QCDfitHist+"/QCDVar/Boson/TrIso/"+m_QCDfitHist;
        fullNameRecTemplNoShift2 ="All/NoShift/"+m_boson+"/QCD/"+m_QCDfitHist+"/QCDTempl/Boson/"+m_QCDfitHist;
    }
    //    string fullNameRecNoShift="All/NoShift/"+m_boson+"/"+m_path+"/"+m_QCDfitHist;i
    string fullNameGen = "All/NoShift/NoCuts/GenInfo/ZPosVtx";


    string bkgListName="QCDName.txt";
    double templateNumCentr=0, templateStatCentr=0;
    map<string, double> templateSysUp, templateSysDown;
    map<string, double> qcdBinning; 
    TH1D* qcdTemplCentr=NULL;
    //string a1[5]={"", "BigBin", "BigRange","SmallBin", "SmallRange"};
    string a1[1]={"Fit"};
    //vector<string> names(a1, a1+5);
    vector<string> names(a1, a1+1); 
    for (std::vector<string>::iterator syst=m_systlist.begin(); syst!=m_systlist.end();++syst){
        for (std::vector<string>::iterator name = names.begin(); name!=names.end(); ++name){
            TH1D* dataHist = (TH1D*)dataFile->Get((fullNameRecFitNoShift+*name).c_str());

            if (dataHist == NULL){
                std::cout << "Calc QCD failed to open datafile" <<std::endl;
                return;
            }

            TAxis* xAxis = dataHist->GetXaxis();
            int nBins = xAxis->GetNbins();
            double qcdCutBin =  (m_QCDcut-xAxis->GetXmin())*nBins/(xAxis->GetXmax()-xAxis->GetXmin());


            TH1D* dataTempl = (TH1D*)dataFile->Get((fullNameRecTemplNoShift+*name).c_str());

            double dataEv = dataHist->Integral();
            RooRealVar x("x","x", dataHist->GetXaxis()->GetXmin(), dataHist->GetXaxis()->GetXmax());   
            RooDataHist data("data", "data" , x, dataHist); 

            if (*syst != "NoShift" && *name != "")
                continue;
            double templateNum, templateSt;
            string fullNameRecFit="All/"+*syst+"/"+m_boson+"/QCD/"+m_QCDfitHist+"/QCDFit/"+m_path+"/"+m_QCDfitHist+*name;
            string fullNameRecTempl = "All/"+*syst+"/"+m_boson+"/QCD/"+m_QCDfitHist+"/QCDTempl/Boson/"+m_QCDfitHist+*name;
            string fullNameRecTempl2="All/"+*syst+"/"+m_boson+"/QCD/"+m_QCDfitHist+"/QCDVar/Boson/TrIso/"+m_QCDfitHist+*name;
            TH1D* dataBuf;
            if (m_boson == "Wmu") {
                //fullNameRecTempl = "All/"+*syst+"/"+m_boson+"/QCD/"+m_QCDfitHist+"/QCDVar/Boson/TrIso/"+m_QCDfitHist+*name;
                //fullNameRecTempl2 = "All/"+*syst+"/"+m_boson+"/QCD/"+m_QCDfitHist+"/QCDTempl/Boson/"+m_QCDfitHist+*name;
                //dataBuf=(TH1D*)dataFile->Get((fullNameRecTemplNoShift2+*name).c_str());
                //dataTempl->Add(dataBuf);
            }
            std::cout << dataTempl->Integral(0, -1) << std::endl;

            string fullNameRecFitNoShift2=fullNameRecFitNoShift+(*name);

            int numb = 0;
            RooArgList* sig = new RooArgList();
            RooArgList* fracs = new RooArgList();


            //setting fit parameters
            ifstream bkgList;
            string procName;
            double xSec, err;
            bkgList.open(bkgListName.c_str());

            int numberOfFiles =count(istreambuf_iterator<char>(bkgList),istreambuf_iterator<char>(), '\n');
            bkgList.seekg(0, ios::beg);

            RooDataHist* rooHists[20];
            RooRealVar* fractions[20];
            RooHistPdf* pdfs[20];
            Double_t css[20];

            bkgList.seekg(0, ios::beg);
            TH1D** mcHists = (TH1D**)malloc(sizeof(TH1D*)*numberOfFiles);

            TH1D* hTotTempl = (TH1D*)dataTempl->Clone(("totMCTempl"+*name).c_str());
            EraseBins(hTotTempl, hTotTempl->GetNbinsX()+1);
            TH1D* hTot = (TH1D*)dataTempl->Clone(("totMC"+*name).c_str());
            EraseBins(hTot, hTot->GetNbinsX()+1);

            //Reading MC
            int i=0;
            double fracSum = 0;
            //           cout << " Lumi = " << m_lumi << endl;
            while (bkgList.good()){
                bkgList >> procName >> xSec >> err;
                string filename = "out_"+procName+".root";
                TFile* bkgFile = TFile::Open(filename.c_str());
                double totGen = ((TH1D*)bkgFile->Get(fullNameGen.c_str()))->GetSumOfWeights();
                double scale = xSec*m_lumi/totGen;
                //cout << "Process " << procName << endl;               
                // cout << "x-sec  " << xSec << " " << totGen << endl;
                //                cout << "scale = " << scale << endl;

                TH1D* hist;
                if (*syst != "NoShift" && bkgFile->Get(fullNameRecFit.c_str()) == NULL) {
                    hist=(TH1D*)bkgFile->Get(fullNameRecFitNoShift2.c_str());
                } else hist = (TH1D*)bkgFile->Get(fullNameRecFit.c_str());

                TH1D* templHist;
                if(*syst != "NoShift" && bkgFile->Get(fullNameRecTempl.c_str()) == NULL) {
                    templHist = (TH1D*)bkgFile->Get((fullNameRecTemplNoShift+*name).c_str());
                } else templHist = (TH1D*)bkgFile->Get((fullNameRecTempl).c_str());

                TH1D* templHist2;

                if (m_boson == "Wmu") {
                    /* if(*syst != "NoShift" && bkgFile->Get(fullNameRecTempl.c_str()) == NULL) {
                       templHist2 = (TH1D*)bkgFile->Get((fullNameRecTemplNoShift2+*name).c_str());
                       } else templHist2 = (TH1D*)bkgFile->Get((fullNameRecTempl2).c_str());
                       templHist->Add(templHist2);*/
                }
                //cout << hist->Integral() << " " << hTot->Integral() << endl;
                //hist->Scale(scale);
                hTot->Add(hist, scale);
                hTotTempl->Add(templHist, scale);
                //cout << hist->Integral() << " " << hTot->Integral() << endl;

                //double nhist = hist->GetSumOfWeights();
                /*
                   double nhist = hist->Integral();
                   if (nhist < 1) 
                   continue;
                   double frac = nhist/dataEv;

                   css[i]=xSec/frac;
                   mcHists[i]=hist;
                   rooHists[i]= new RooDataHist(procName.c_str(), procName.c_str(), x, mcHists[i]);
                   Double_t minFr = frac*(1-err);
                   Double_t maxFr = frac*(1+err);
                   TString fracName="f_"+procName;
                   RooRealVar fsig(fracName, fracName, frac, minFr, maxFr);
                   fracSum+=frac;
                   fractions[i] = new RooRealVar(fsig);
                   if (err == 0)
                   fractions[i]->setConstant(kTRUE);
                   TString pdfName = procName+"_PDF";
                   RooHistPdf mcPdf (pdfName, pdfName, RooArgSet(x), *rooHists[i], 0);
                   pdfs[i]= new RooHistPdf(mcPdf);
                   sig->add(*pdfs[i]);
                   fracs->add(*fractions[i]);*/
                numb++;
                i++;
            }
            double nhist=hTot->Integral();
            double frac = nhist/dataEv;
            RooDataHist* histEwk = new RooDataHist("Ewk", "Ewk", x, hTot);
            if (frac > 1)
                frac = 0.99;
            RooRealVar fracEwk("EwkFrac", "EwkFrac", 0.99, 0.95, 1.0);
            RooHistPdf mcPdf("EwkPDF", "EwkPDF", RooArgSet(x), *histEwk, 0);
            sig->add(mcPdf);
            fracs->add(fracEwk);
            TH1D* qcdHist= new TH1D(*dataTempl);
            qcdHist->Add(hTotTempl, -1);
            if (m_boson == "Wmu"){
                //int t=qcdHist->FindBin(50.0);
                //EraseBins(qcdHist, t, true);
                //EraseBins(qcdHist);

                //int t=qcdHist->FindBin(50.0);
                //EraseBins(qcdHist, t, true);
                //
                TFile* fileBB=new TFile("out_bbTomu15.root");
                TFile* fileCC=new TFile("out_ccTomu15.root");
                TH1D* hist1=(TH1D*)fileBB->Get((fullNameRecTemplNoShift+*name).c_str());
                TH1D* hist2=(TH1D*)fileCC->Get((fullNameRecTemplNoShift+*name).c_str());

                //TH1D* hist1=(TH1D*)fileBB->Get((fullNameRecFitNoShiftComb+*name).c_str());
                //TH1D* hist2=(TH1D*)fileCC->Get((fullNameRecFitNoShiftComb+*name).c_str());
                hist1->SetDirectory(0);
                hist2->SetDirectory(0);
                qcdHist=hist1;
                qcdHist->Add(hist2);
                fileBB->Close();
                fileCC->Close();

            } else if (m_path == "Minus"){
                int t=qcdHist->FindBin(30.0);
                //EraseBins(qcdHist,t, true);
            } else {
                int t=qcdHist->FindBin(40.0);
                //EraseBins(qcdHist,t, true);
            }

            EraseBins(qcdHist);
            templateNum = qcdHist->Integral(0, nBins);
            double errTempl;
            templateSt = qcdHist->IntegralAndError(qcdCutBin, nBins, errTempl); 
            templateSt = errTempl/templateSt;        

            TH1D* templDiff = new TH1D(*qcdHist);
            //filling checking histograms to file
            if ((*syst) == "NoShift") {
                topDir->cd();
                qcdHist->Write();
                TCanvas* b = new TCanvas(("Data"+*name).c_str());
                dataHist->Draw();
                b->Write();
                TCanvas* c = new TCanvas(("Template"+*name).c_str(), ("Template for "+m_boson).c_str());
                dataTempl->GetXaxis()->SetName(m_QCDfitHist.c_str());
                dataTempl->Draw();
                dataTempl->SetLineColor(kRed);
                hTotTempl->Draw("same");
                TLegend* leg = new TLegend(0.63,0.525,0.87,0.875);
                leg->AddEntry(dataTempl, "data");
                leg->AddEntry(hTotTempl, "mc");
                leg->Draw();
                c->Write();
                hTot->Write();
                if (*name == ""){
                    qcdTemplCentr=(TH1D*)qcdHist->Clone("qcdCentr");
                }
            } else {
                templDiff->Add(qcdTemplCentr, -1.0);
                templDiff->SetName((*syst+"templDiff").c_str());
                topDir->cd();
                templDiff->Write();
            }

            //adding QCD sample
            RooDataHist qcd("QCD", "QCD", x, qcdHist);
            RooHistPdf qcdPdf("qcdPDF", "qcd PDF", RooArgSet(x), qcd, 0);
            sig->add(qcdPdf);

            //making fit	
            RooAddPdf model ("model", "MC+qcd", *sig, *fracs);
            model.fitTo(data, PrintEvalErrors(-1));
            if (*name == ""){
                //plotting and saving result of fit
                TDirectory* systDir = topDir->mkdir((*syst).c_str());
                systDir->cd();
            }
            RooPlot* frame = x.frame();
            frame->SetName(("frame"+*name+*syst).c_str());
            data.plotOn(frame);
            model.plotOn(frame);
            model.plotOn(frame, Components("EwkPDF"), LineStyle(kDashed)); 
            model.plotOn(frame, Components("qcdPDF"), LineStyle(kDashed));
            frame->Write();
            topDir->cd();


            //calculating fraction of QCD events
            double qcdNum =0;
            double qcdFitErr = 0;

            /*            for (int i=0; i<numb; i++){
                          qcdNum+=fractions[i]->getVal();
                          qcdFitErr+=fractions[i]->getError();
                          }
                          fracEwk
                          */          
            qcdNum=1-fracEwk.getVal();
            qcdFitErr=fracEwk.getError();
            qcdNum*=dataHist->Integral(1, nBins);
            if (*syst=="NoShift"){
                std::cout << "Total number of QCD events before cut for "<< *name << " = " << qcdNum << " +-" << qcdFitErr*qcdNum << std::endl;
            }

            //calculating total number of QCD events after cut
            qcdHist->Scale(qcdNum/qcdHist->Integral(1, nBins));
            qcdNum = qcdHist->Integral(qcdCutBin, nBins);
            qcdFitErr*=qcdNum;

            if (*name == "Fit"){
                m_QCDcutBin = qcdCutBin;   
            }
            if (*syst == "NoShift"){
                std::cout << "Total number of QCD events after cut = ";
                std::cout << qcdNum << " +-" <<qcdNum*qcdFitErr << std::endl;
            }




            //saving everything for systematics calculation
            if (*syst  == "NoShift"){
                if (*name == "Fit"){
                    templateNumCentr = templateNum;
                    templateStatCentr = templateSt;
                    qcdNumCentr=qcdNum;
                    //qcdUp["Fit range"]=qcdNumBig;
                    //qcdDown["Fit range"]=qcdNumSmall;
                    //templateSysUp["Fit range"]=0;
                    //templateSysDown["Fit range"]=0;
                    qcdErrFit = qcdNum*qcdFitErr;
                }else {
                    bool Up = ( (*name).rfind("Big") !=std::string::npos);
                    bool Down = ( (*name).rfind("Small") !=std::string::npos);            
                    string subname = Up ? (*name).substr((*name).rfind("Big")+3) : (*name).substr((*name).rfind("Small")+5);
                    //cout << subname << " "<< Up << " " <<Down << endl;
                    if (Up){
                        templateSysUp[subname]=0.0;
                        qcdUp[subname]=qcdNum;
                    }
                    if (Down){
                        templateSysDown[subname]=0.0;
                        qcdDown[subname]=qcdNum;
                    }
                }

                //qcdErrStat
            } else { 
                bool Up = ( (*syst).rfind("Up") == (*syst).length()-2);
                bool Down = ( (*syst).rfind("Down") == (*syst).length()-4);

                if ( ! (Up || Down) ) {
                    std::cout << "NOT up-down variation ! "<<std::endl;
                    continue;
                }
                double rms=templDiff->GetRMS(2);
                string subname = Up ? (*syst).substr(0,(*syst).rfind("Up")) : (*syst).substr(0,(*syst).rfind("Down"));
                if ( Up ) {
                    templateSysUp[subname] = rms;
                    qcdUp[subname] = qcdNum;
                } 
                if ( Down ) {
                    templateSysDown[subname] = rms;
                    qcdDown[subname] = qcdNum;
                }
            }
        }
    }

    std::cout << "\n\n\n" ;
    dataFile->Close();
    MakeQCDFile(m_boson, m_path, qcdNumCentr); 

    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
    std::cout << "Calculating QCD for " << std::endl;
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

    fitResults->Write();
    fitResults->Close();
    templateStatCentr=qcdNumCentr*templateStatCentr;
    //filling QCD estimation tables
    ofstream SystTemplTableOut, SystFitTableOut, SystTotalOut;
    string systTableOutName = "QCDSystematics/"+m_nameBos+"/"+m_nameBos+"QCDTemplateSystTable.tex";
    string systUncertOutName = "QCDSystematics/"+m_nameBos+"/"+m_nameBos+"QCDFitSystTable.tex";
    string systQCDTotalName = "QCDSystematics/"+m_nameBos+"/"+m_nameBos+"QCDTotalSyst.tex";
    SystTemplTableOut.open(systTableOutName.c_str());
    SystFitTableOut.open(systUncertOutName.c_str());
    SystTotalOut.open(systQCDTotalName.c_str());

    double totQCD = (qcdNumCentr-qcdNumCentr2)/qcdNumCentr;

    SystTemplTableOut<<"Statistics  &  "<<fixed<<setprecision(2)<<templateStatCentr <<"  &  "<<-templateStatCentr<<"  \\\\"<<endl;
    SystFitTableOut << "NumberOfQCDEvents " << fixed << setprecision(2) << qcdNumCentr << " & " << " \\\\" << endl;
    SystFitTableOut << "FitMethod " << fixed <<  setprecision(2)<< totQCD << " & " << " \\\\" << endl;
    SystFitTableOut << "FitError & " << fixed << setprecision(2)<<qcdErrFit << " & " << -qcdErrFit << " \\\\" << endl;
    //   SystFitTableOut << "FitError2 & " << fixed << setprecision(2)<<qcdErrFit2 << " & " << -qcdErrFit2 << " \\\\" << endl;





    printf("%55s %10s %10s \n", "Source", "Up", "Down"); 
    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;

    printf("%55s %10.2f %10.2f \n","Statistics", templateStatCentr, -templateStatCentr);   
    printf("%55s %10.2f %10.2f \n", "Fit error", qcdErrFit, qcdErrFit);	
    // printf("%55s %10.2f %10.2f \n", "Fit error", qcdErrFit2, qcdErrFit2);	

    double dE = 0;
    double uE = 0;

    //calculating files for template systematics

    double uT=0, dT=0;

    //other fir systematics
    for (std::map<std::string,double>::iterator pair = qcdUp.begin(); pair != qcdUp.end(); ++ pair){ 

        string name = (*pair).first;

        double u = (qcdUp[name]-qcdNumCentr);
        double d = (qcdDown[name]-qcdNumCentr);


        double utempl = (templateSysUp[name])/templateNumCentr*100.;
        double dtempl = (templateSysDown[name])/templateNumCentr*100.;


        printf("%55s %10.2f %10.2f \n",name.c_str(),
                u,
                d
              );

        if ( (d < 0) && (u<0) ) {
            dE +=  u<d ?  u*u : d*d;
        }

        else if ( (d<0) && ( u>0 ) ) {
            dE += d*d;
            uE += u*u;
        }
        else if ( (d>0) && ( u<0 ) ) {
            dE += u*u;
            uE += d*d;
        }
        else {
            uE +=  u>d ?  u*u : d*d;
        }

        SystFitTableOut<< name <<"  &  "<<fixed<<setprecision(2)<<u<<"  &  "<<d<<"  \\\\"<<endl;

        if ( (dtempl < 0) && (utempl<0) ) {
            dT +=  utempl<dtempl ?  utempl*utempl : dtempl*dtempl;
        }
        else if ( (dtempl<0) && ( utempl>0 ) ) {
            dT += dtempl*dtempl;
            uT += utempl*utempl;
        }
        else if ( (dtempl>0) && ( utempl<0 ) ) {
            dT += utempl*utempl;
            uT += dtempl*dtempl;
        }
        else {
            uT +=  utempl>dtempl ?  utempl*utempl : dtempl*dtempl;
        }
        SystTemplTableOut<< name <<"  &  "<<fixed<<setprecision(2)<<utempl<<"  &  "<<dtempl<<"  \\\\"<<endl;

    }

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    printf("%55s %10.2f %10.2f \n","Total Fit",
            sqrt(uE),
            -sqrt(dE)
          );
    printf ("%55s %10.2f %10.2f \n","Total Template",
            sqrt(uT),
            -sqrt(dT)
           );
    printf ("%55s %10.2f %10.2f \n","Statistics",
            templateStatCentr,
            -templateStatCentr
           );
    uT+=templateStatCentr*templateStatCentr;
    dT+=templateStatCentr*templateStatCentr;

    std::cout << "-------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Number of QCD events = " << qcdNumCentr << " +" << sqrt(uE) << "(" << -sqrt(dE)<<")" << "(syst)" << " +- " << sqrt(uT) << "(stat)" << std::endl;
    /*    string filename = "QCDSystematics/"+m_nameBos+".out";
          FILE* sysOut=fopen(filename.c_str(), "w");
          fprintf(sysOut, "%10.2f %10.4f", qcdNumCentr, sqrt(uE), -sqrt(dE));
          fclose(sysOut);
          */
    /*
       string SystTableName = "SystematicsTexFormat/"+m_nameBos+"/"+m_nameBos+"SystTable.tex";
       string SystForDataName = "SystematicsDatFormat/"+m_nameBos+"/C_"+m_nameBos+"_syst.dat";
       string SystIncertName = "SystematicsTexFormat/"+m_nameBos+"/"+m_nameBos+"SystValues.tex";	

       SystTableOut.open(SystTableName.c_str(), std::ios::app);
       SystForDatOut.open(SystForDataName.c_str(), std::ios::app);
       SystUncertOut.open(SystIncertName.c_str(), std::ios::app);
       */


    std::cout << "\n\n\n";
}

void ZeeDPlotter::GetXSection() 
{
    // Hardwire tables of MC cross sections and filter efficiencies:

    if (m_lumi == 0) {
        std::cout << " No luminosity, stop x-section calculation \n";
        return;
    }

    TFile* dataFile = TFile::Open(m_dataname.c_str());
    if ( dataFile == NULL) {
        std::cout << " Failed to open data file, stop x-section calculation \n";
        return;
    }


    string fullNameTot = "All/NoShift/NoCuts/GenInfo/ZPosVtx";
    string fullNameRec = "All/NoShift/"+m_boson+"/"+m_path+"/"+m_refHistogramRec;

    TH1* hrec = (TH1*)dataFile->Get(fullNameRec.c_str());

    if (hrec == NULL) {
        std::cout << " Did not find data histogram, stop x-section calculation \n";
        return;
    }

    double dataEvents = hrec->GetSumOfWeights();
    std::cout << "Data events = " << dataEvents <<std::endl;

    ifstream bkgList;
    string bkgListName="BkgName.txt";
    bkgList.open(bkgListName.c_str());
    bkgList.seekg(0, ios::beg);
    string procName;
    double err;

    double totalBkgNum=0;
    double xSec, err2;
    bkgList >> procName >> xSec >> err2;
    double diBosons=0, diBosonsSt=0, ttbar=0, ttbarSt=0, WBosons=0, WBosonsSt=0, QCDSyUp=0,QCDSyDown=0, QCDSt=0;
    double ZBosons=0, ZBosonsSt=0, DY=0, DYSt=0, QCD=0, ZBosonsSy=0, WBosonsSy=0;
    std::cout << "fullNameRec " << fullNameRec << "  tot " << fullNameTot << endl;
    while (bkgList.good()){
        if (procName.find(m_nameBos) != std::string::npos){
            bkgList >>procName>>xSec>>err2;
            std::cout << "Finding another " << procName << endl;
            continue;
        }
        string filename = "out_"+procName+".root";
        TFile* bkgFile = TFile::Open(filename.c_str());
        if (bkgFile != NULL){
            TH1* hrec = (TH1*)bkgFile->Get(fullNameRec.c_str());
            TH1* htot = (TH1*)bkgFile->Get(fullNameTot.c_str());
            if (hrec == NULL) {
                std::cout << " Did not find " << procName << " histogram, stop x-section calculation \n";
                return;
            }
            if (htot == NULL) {
                std::cout << " Did not find " << procName << " histogram, stop x-section calculation \n";
                return;
            }
            double tot = htot->GetSumOfWeights();        
            double rec = hrec->GetSumOfWeights();

            hrec->IntegralAndError(0,1000, err); 
            double	bkgSt = rec>0 ? err/rec * 100. : 0.;        
            double bkgNum= tot != 0 ? (rec/tot) * xSec * m_lumi : 0;
            double bkgSy = bkgNum*err2;        
            std::cout << procName << " background: " << bkgNum << "+-" << bkgNum * bkgSt/100. << std::endl;
            totalBkgNum+=bkgNum;

            if (procName == "WW" || procName == "WZ" || procName == "ZZ") {
                diBosons+=bkgNum;
                diBosonsSt+=pow(bkgSt*bkgNum/100.,2);
            } else if (procName == "ttbar"){
                ttbar +=bkgNum; 
                ttbarSt +=pow(bkgSt*bkgNum/100.,2);
            } else if (procName.find("W") != std::string::npos){ 
                WBosons+=bkgNum;
                WBosonsSt+=pow(bkgSt*bkgNum/100.,2);
                WBosonsSy+=pow(bkgSy, 2);
            } else if (procName.find("Z") != std::string::npos){
                ZBosons+=bkgNum;
                ZBosonsSt+=pow(bkgSt*bkgNum/100.,2);
                ZBosonsSy+=pow(bkgSy, 2);
            } else if (procName.find("DY") != std::string::npos){
                DY+=bkgNum;
                DYSt+=pow(bkgSt*bkgNum/100.,2);
            }
            delete bkgFile;
        }
        bkgList >>procName>>xSec >> err2;
        std::cout << procName << std::endl;
    }
    if (m_boson == "W" || m_boson == "Wmu"){
        string filename = "QCDSystematics/"+m_nameBos+".out";
        ifstream qcdFile;
        qcdFile.open(filename.c_str());
        qcdFile.seekg(0, ios::beg);
        qcdFile >> QCD >> QCDSyUp;
        std::cout << "QCD = " << QCD << " +- " << QCDSyUp <<std::endl;
        totalBkgNum+=QCD;
    }

    ttbarSt=sqrt(ttbarSt);
    diBosonsSt=sqrt(diBosonsSt);
    WBosonsSt=sqrt(WBosonsSt);
    ZBosonsSt=sqrt(ZBosonsSt);
    DYSt=sqrt(DYSt);

    double ewkBg      = diBosons + WBosons + ZBosons + ttbar+ DY;
    double ewkErrStat = sqrt ( pow(diBosonsSt,2.0) + pow(WBosonsSt,2.0) + pow(ZBosonsSt,2) + pow(ttbarSt, 2)+ pow(DYSt, 2.0));

    // Systematics for EWK
    double diBosonsSy = diBosons*diBosonsSt/100.;
    WBosonsSy  = sqrt(WBosonsSy+pow(WBosons*WBosonsSt/100., 2));
    ZBosonsSy  = sqrt(ZBosonsSy+pow(ZBosons*ZBosonsSt/100., 2));
    double ttbarSy    = ttbar*ttbarSt/100.;
    double DYSy       = DY*DYSt/100.;
    //QCDSyUp=QCDSyUp/m_czvalue/m_lumi;
    //QCDSyDown=QCDSyDown/m_czvalue/m_lumi;

    double dataSub = dataEvents-totalBkgNum;
    double ewkErrSyst = sqrt ( diBosonsSy*diBosonsSy + WBosonsSy*WBosonsSy + ZBosonsSy*ZBosonsSy + ttbarSy*ttbarSy + DYSy*DYSy+pow(m_lumi_err*ewkBg,2));
    double ewkErrRel =ewkErrSyst*100./dataSub;
    double qcdErrRel =QCDSyUp*100./dataSub;
    double bkgErrTotal=sqrt(ewkErrSyst*ewkErrSyst+QCDSyUp*QCDSyUp);
    std::cout << "Total number of events " << std::endl;
    std::cout << dataEvents << " - " << ewkBg << " - " << QCD << std::endl;
    std::cout << " Signal events " << std::endl;
    std::cout << dataSub << "+-" << ewkErrSyst  << "+-" << QCDSyUp << endl; 


    if ( m_czvalue==-1)
        return;
    double dataStatErr=1/sqrt(dataEvents)*100;
    double sigma = 1000*dataSub/m_lumi/ m_czvalue;
    double sigmaStat = 1000*sqrt(dataEvents) /m_lumi/ m_czvalue;
    double sigmaSysPlus = sqrt(m_czsystUp*m_czsystUp+ewkErrRel*ewkErrRel)*sigma/100.;
    double sigmaSysMinus = sqrt(m_czsystDown*m_czsystDown+ewkErrRel*ewkErrRel)*sigma/ 100.;
    double sigmaSym = 0.5*(fabs(sigmaSysPlus) +fabs(sigmaSysMinus));
    double sigmaLumi = m_lumi_err * sigma;

    double sigmaTot = sigma/m_Az;
    double sigmaTotStat = sigmaStat/m_Az;
    double sigmaTotSym = sigmaTot*sigmaSym/sigma;
    double sigmaTotEx= m_AzErr*sigmaTot/100.;
    //double sigmaTotSym = sqrt(pow(sigmaSym/sigma,2)+pow((m_AzErr/100.),2))*sigmaTot;
    double sigmaTotLumi = sigmaTot * m_lumi_err;


    double sigma13 = sigma/m_Ez;
    double sigma13Stat = sigmaStat/m_Ez;
    double sigma13Sym = sigma13*sigmaSym/sigma;
    double sigma13Ex= m_EzErr*sigma13/100.;
    //double sigmaTotSym = sqrt(pow(sigmaSym/sigma,2)+pow((m_AzErr/100.),2))*sigmaTot;
    double sigma13Lumi = sigma13 * m_lumi_err;


    std::cout << "Fiducial cross section = " << sigma  << " +- " << sigmaStat << "(stat) + " << sigmaSysPlus << "(-" << sigmaSysMinus << ") (syst) " << sigmaLumi << std::endl;
    std::cout << "Total cross section = " << sigmaTot  << " +- " << sigmaTotStat << " +- " << sigmaTotSym <<"  "<< sigmaTotLumi << std::endl;
    std::cout << sigmaSym << std::endl;

    FILE * texOut;
    FILE * datOut;
    FILE * bgOut;


    string texOutName ="SystematicsTexFormat/"+m_nameBos+"/"+m_nameBos+"_Results.tex";
    string datOutName ="SystematicsDatFormat/"+m_nameBos+"/"+m_nameBos+"_sigma_fidu.dat";
    string bgOutName ="SystematicsDatFormat/"+m_nameBos+"/"+m_nameBos+"Background_syst.dat";
    texOut  = fopen(texOutName.c_str(),"w");
    datOut  = fopen(datOutName.c_str(),"w");
    bgOut   = fopen(bgOutName.c_str(),"w");

    // Sigma fidu
    //    fprintf( texOut, "\\newcommand{\\sigfid%s}{\\ensuremath{[ %6.1f \\pm %6.1f \\mathrm{(stat)} \\pm %6.1f \\mathrm{(syst)} \\pm %6.1f \\mathrm{(lumi)}]\\;\\mathrm{nb}}}\n",
    //            m_nameBos.c_str(), this->ceilNN(sigma, 1), this->ceilNN(sigmaStat, 1), this->ceilNN(sigmaSym, 1), this->ceilNN(sigmaLumi, 1));
    fprintf( texOut, "\\newcommand{\\sigfid%snolabel}{\\ensuremath{ %6.1f \\pm %6.1f  \\pm %6.1f  \\pm %6.1f  }} \n",
            m_nameBos.c_str(), this->ceilNN(sigma,1), this->ceilNN(sigmaStat,1), this->ceilNN(sigmaSym,1), this->ceilNN(sigmaLumi,1));
    //    fprintf( texOut, "\\newcommand{\\sig%snolabel}{\\ensuremath{ %6.1f \\pm %6.1f  \\pm %6.1f  \\pm %6.1f  }}\n",
    //            m_nameBos.c_str(), this->ceilNN(sigmaTot,1), this->ceilNN(sigmaTotStat,1), this->ceilNN(sigmaTotSym,1), this->ceilNN(sigmaTotLumi,1)); 

    // Sigma tot
    //    fprintf( texOut, "\\newcommand{\\sigtot%s}{\\ensuremath{[ %6.1f \\pm %6.1f \\mathrm{(stat)} \\pm %6.1f \\mathrm{(syst)} \\pm %6.1f \\mathrm{(lumi)} \\pm \\mathrm{(ex)}]\\;\\mathrm{nb}}}\n",
    //            m_nameBos.c_str(), this->ceilNN(sigmaTot,1), this->ceilNN(sigmaTotStat,1), this->ceilNN(sigmaTotSym,1), this->ceilNN(sigmaTotLumi,1), this->ceilNN(sigmaTotEx,1));
    fprintf( texOut, "\\newcommand{\\sigtot%snolabel}{\\ensuremath{ %6.1f \\pm %6.1f  \\pm %6.1f  \\pm %6.1f \\pm %6.1f  }} \n",
            m_nameBos.c_str(), this->ceilNN(sigmaTot,1), this->ceilNN(sigmaTotStat,1), this->ceilNN(sigmaTotSym,1), this->ceilNN(sigmaTotLumi,1), this->ceilNN(sigmaTotEx,1));

    fprintf( texOut, "\\newcommand{\\sigTr%snolabel}{\\ensuremath{ %6.1f \\pm %6.1f  \\pm %6.1f  \\pm %6.1f   }} \n",
            m_nameBos.c_str(), this->ceilNN(sigma13,1), this->ceilNN(sigma13Stat,1), this->ceilNN(sigma13Sym,1), this->ceilNN(sigma13Lumi,1));


    // C
    fprintf( texOut, "\\newcommand{\\systErr%s}{%7.4f} \n", m_nameBos.c_str(), m_czsystUp);

    fprintf( texOut, "\\newcommand{\\stat%s}{%7.2f}\n", m_nameBos.c_str(), dataStatErr);
    fprintf( texOut, "\\newcommand{\\ewk%s}{%7.2f}\n", m_nameBos.c_str(), ewkErrRel);
    fprintf( texOut, "\\newcommand{\\qcd%s}{%7.2f}\n", m_nameBos.c_str(), qcdErrRel);
    fprintf( texOut, "\\newcommand{\\C%s}{%7.4f}\n", m_nameBos.c_str(), m_czvalue);
    fprintf( texOut, "\\newcommand{\\C%sStatErr}{%7.4f} \n", m_nameBos.c_str(), m_czstat/100.*m_czvalue);
    fprintf( texOut, "\\newcommand{\\C%sSystUpErr}{%7.4f} \n", m_nameBos.c_str(), m_czsystUp/100.*m_czvalue);
    fprintf( texOut, "\\newcommand{\\C%sSystDownErr}{%7.4f} \n", m_nameBos.c_str(), m_czsystDown/100.*m_czvalue);
    fprintf(texOut, "\\newcommand{\\C%sSystErr}{%7.4f} \n", m_nameBos.c_str(), (m_czsystUp-m_czsystDown)/2);
    // Event yields:
    fprintf( texOut, "\\newcommand{\\ntot%s}{%10.0f} \n",   m_nameBos.c_str(), dataEvents);
    //        if (diBosons != 0.0)
    fprintf( texOut, "\\newcommand{\\nDiboson%s}{%6.2f}\n", m_nameBos.c_str(), diBosons/ dataEvents * 100.);
    //        if (WBosons != 0.0)
    fprintf( texOut, "\\newcommand{\\nW%s}{%6.2f}\n",   m_nameBos.c_str(),  WBosons / dataEvents * 100.);
    //        if (ZBosons != 0.0)
    fprintf( texOut, "\\newcommand{\\nZ%s}{%6.2f} \n", m_nameBos.c_str(), ZBosons / dataEvents * 100.);
    //        if (ttbar != 0.0)
    fprintf( texOut, "\\newcommand{\\nttbar%s}{%6.2f} \n",   m_nameBos.c_str(), ttbar / dataEvents * 100.);       
    //       if (DY != 0.0)
    fprintf( texOut, " \\newcommand{\\nDY%s}{%6.2f} \n",   m_nameBos.c_str(), DY / dataEvents * 100.);       
    fprintf( texOut, " \\newcommand{\\nQCD%s}{$%6.2f \\pm %8.1f $} \n",   m_nameBos.c_str(), QCD, QCDSyUp);       

    fprintf( texOut, "\\newcommand{\\nEWttbarbkg%s}{$ %6.1f  \\pm %6.1f $} \n",
            m_nameBos.c_str(), ewkBg, ewkErrSyst);
    if (m_boson == "W" || m_boson == "Wmu"){
        fprintf( texOut, "\\newcommand{\\ntotsignal%s}{$ %8.1f \\pm %8.1f \\pm %8.1f \\pm %8.1f $} \n",
                m_nameBos.c_str(), dataSub, sqrt(dataEvents), ewkErrSyst, QCDSyUp);
        fprintf( texOut, "\\newcommand{\\ntotsignalD%s}{$ %8.1f \\pm %8.1f \\pm %8.1f $} \n",
                m_nameBos.c_str(), dataSub, ewkErrSyst, QCDSyUp);
    } else{
        fprintf( texOut, "\\newcommand{\\ntotsignal%s}{$ %8.1f \\pm %8.1f \\pm %8.1f $} \n",
                m_nameBos.c_str(), dataSub, sqrt(dataEvents), ewkErrSyst);
        fprintf( texOut, "\\newcommand{\\ntotsignalD%s}{$ %8.1f \\pm %8.1f  $} \n",
                m_nameBos.c_str(), dataSub, ewkErrSyst);
    }



    fclose(texOut);

    // Data file:
    fprintf(datOut, "Sigma_fid %15s %8.5f %8.5f \n", m_nameBos.c_str(), sigma, sigmaStat);
    fclose(datOut);

    // BG file:
    fprintf(bgOut, "BkgTotal %8.4f \n", bkgErrTotal/dataEvents*100);
    fprintf(bgOut, "Top  %8.4f %8.4f \n", ttbarSy / dataEvents*100., -ttbarSy / dataEvents*100.  );
    fprintf(bgOut, "WBos %8.4f %8.4f \n", WBosonsSy / dataEvents*100., -WBosonsSy / dataEvents*100.  );
    fprintf(bgOut, "Z %8.4f %8.4f \n", ZBosonsSy / dataEvents*100., -ZBosonsSy / dataEvents*100.  );
    fprintf(bgOut, "DiBos %8.4f %8.4f \n", diBosonsSy / dataEvents*100., -diBosonsSy / dataEvents*100.  );
    fprintf(bgOut, "DY %8.4f %8.4f \n", DYSy / dataEvents*100., -DYSy / dataEvents*100.  );
    fprintf(bgOut, "QCD %8.4f %8.4f \n", QCDSyUp/dataEvents*100., -QCDSyUp/dataEvents*100.);
    fprintf(bgOut, "EWKBkg %8.4f %8.4f \n", ewkErrSyst/dataEvents*100., -ewkErrSyst/dataEvents*100.);
    fprintf(bgOut, "BgEWKLumi %8.4f %8.4f \n", m_lumi_err*ewkBg / dataEvents, -m_lumi_err*ewkBg/ dataEvents);
    fclose(bgOut);
    cout << endl << endl << endl;

    return;
}


double ZeeDPlotter::ceilNN(double v, int p)
{
    v *= pow(10, p);
    v = round(v);
    v /= pow(10, p);
    return v;
}
// ----------------------------------------------------
