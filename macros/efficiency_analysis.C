#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TH2D.h>
#include <TStyle.h>
#include <iostream>
#include <vector>
#include <string>

void efficiency_analysis() {
    gStyle->SetOptStat(0);

    // input setup
    std::vector<double> energies = {
        20, 30, 40, 50, 60, 80, 100, 150, 200, 300,
        400, 500, 600, 700, 800, 900, 1000, 1200,
        1400, 1600, 2000, 3000, 4000, 5000,
        6000, 7000, 8000, 9000, 10000
    };

    // storage vectors
    std::vector<double> Ta_vals, TcTt_vals, DE_vals;
    std::vector<double> meanEdep_vals, meanPhotons_vals;

    // loop over all files
    for (size_t i = 0; i < energies.size(); ++i) {

        std::string filename = "output" + std::to_string(i) + ".root";
        TFile *file = TFile::Open(filename.c_str());

        if (!file || file->IsZombie()) {
            std::cerr << "Skipping missing file: " << filename << std::endl;
            continue;
        }

        TTree *tree = (TTree*)file->Get("EventSummary");
        if (!tree) {
            std::cerr << " No EventSummary tree in " << filename << std::endl;
            file->Close();
            continue;
        }

        Double_t absorptionEff, generatedPhotons, escapedPhotons, totalE_keV;
        tree->SetBranchAddress("absorptionEff", &absorptionEff);
        tree->SetBranchAddress("generatedPhotons", &generatedPhotons);
        tree->SetBranchAddress("escapedPhotons", &escapedPhotons);
        tree->SetBranchAddress("totalE_keV", &totalE_keV);

        Long64_t nentries = tree->GetEntries();
        if (nentries == 0) {
            file->Close();
            continue;
        }

        double sumAbsEff = 0, sumGen = 0, sumEsc = 0, sumEdep = 0;

        for (Long64_t j = 0; j < nentries; ++j) {
            tree->GetEntry(j);
            sumAbsEff += absorptionEff;
            sumGen += generatedPhotons;
            sumEsc += escapedPhotons;
            sumEdep += totalE_keV;
        }
        
        double meanEdep = sumEdep / nentries;    // keV/event
        double meanGen  = sumGen  / nentries;    // generated photons/event
        double meanEsc  = sumEsc  / nentries;    // escaped photons/event

        // Absorption efficiency: energy absorbed / incident energy
        double Ta = 0.0;
        if (energies[i] > 0.0)
            Ta = meanEdep / energies[i];

        // Optical yield: escaped photons per deposited keV
        double TcTt = 0.0;
        if (meanEdep > 0.0)
            TcTt = meanEsc / meanEdep;
        
        // Detection efficiency as energy-out / energy-in
        // optical photon energy = 2.5 eV = 2.5e-3 keV (given in paper)
        double Eopt_keV = 2.5e-3;
        double DE = 0.0;
        if (energies[i] > 0.0)
            DE = (meanEsc * Eopt_keV) / energies[i];


        /* original code

        // -------------------------
        double Ta = sumAbsEff / nentries;

        double TcTt;

        if (sumGen > 0) {
            TcTt = sumEsc / sumGen;
        } else {
            TcTt = 0.0;
        }

        double DE = Ta * TcTt;

        double meanEdep = sumEdep / nentries;

        double meanPhotons = sumPhotons / nentries;

        // -------------------
        */

        Ta_vals.push_back(Ta);
        TcTt_vals.push_back(TcTt);
        DE_vals.push_back(DE);
        meanEdep_vals.push_back(meanEdep);
        meanPhotons_vals.push_back(meanGen);

        std::cout << energies[i] << " keV: "
                  << "Ta=" << Ta << ", TcTt=" << TcTt << ", DE=" << DE << std::endl;

        file->Close();
    }

    //  Absorption Efficiency vs Energy

    TCanvas *c1 = new TCanvas("c1", "Absorption Efficiency vs Energy", 800, 600);
    TGraph *gTa = new TGraph(energies.size(), &energies[0], &Ta_vals[0]);
    gTa->SetTitle("Absorption Efficiency (T_{a}) vs Incident Gamma Energy;Energy [keV];Absorption Efficiency (T_{a})");
    gTa->SetLineWidth(2);
    gTa->SetLineColor(kBlue + 2);
    gTa->SetMarkerStyle(21);
    gTa->SetMarkerColor(kBlue + 2);
    gTa->Draw("ALP");
    c1->SetGrid();

    //  Transmission Efficiency (TcTt) vs Energy

    TCanvas *c2 = new TCanvas("c2", "Transmission Efficiency vs Energy", 800, 600);
    TGraph *gTcTt = new TGraph(energies.size(), &energies[0], &TcTt_vals[0]);
    gTcTt->SetTitle("Optical Transmission Efficiency (T_{c}T_{t}) vs Energy;Energy [keV];Transmission Efficiency (T_{c}T_{t})");
    gTcTt->SetLineWidth(2);
    gTcTt->SetLineColor(kGreen + 2);
    gTcTt->SetMarkerStyle(22);
    gTcTt->SetMarkerColor(kGreen + 2);
    gTcTt->Draw("ALP");
    c2->SetGrid();

    //  Detection Efficiency (DE) vs Energy

    TCanvas *c3 = new TCanvas("c3", "Detection Efficiency vs Energy", 800, 600);
    TGraph *gDE = new TGraph(energies.size(), &energies[0], &DE_vals[0]);
    gDE->SetTitle("Detection Efficiency (DE = T_{a}*T_{c}T_{t}) vs Energy;Energy [keV];Detection Efficiency");
    gDE->SetLineWidth(2);
    gDE->SetLineColor(kRed + 1);
    gDE->SetMarkerStyle(20);
    gDE->SetMarkerColor(kRed + 1);
    gDE->Draw("ALP");
    c3->SetGrid();

    //   Photon Yield vs Deposited Energy

    TCanvas *c4 = new TCanvas("c4", "Photon Yield vs Deposited Energy", 800, 600);
    TGraph *gLY = new TGraph(meanEdep_vals.size(), &meanEdep_vals[0], &meanPhotons_vals[0]);
    gLY->SetTitle("Output Photon Number vs Deposited Energy;Deposited Energy [keV];Generated Photons");
    gLY->SetLineWidth(2);
    gLY->SetLineColor(kMagenta + 1);
    gLY->SetMarkerStyle(23);
    gLY->SetMarkerColor(kMagenta + 1);
    gLY->Draw("ALP");
    c4->SetGrid();

    std::cout << "\n All plots generated successfully.\n";
}
