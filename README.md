# Detection Efficiency of a Plastic Scintillating Fiber (PSF) – Geant4 Reproduction

**Author:** Nisant Sarma  

This project reproduces the detection-efficiency curves of a plastic scintillating fiber (PSF) under γ-ray irradiation, based on the paper:

> **S.-B. Tang et al.**,  
> *Simulation study on detection efficiency of plastic scintillating fiber under γ-ray radiation*,  
> Radiation Physics and Chemistry 77 (2008) 115–120.

The repository contains:
- Full Geant4 source code for the detector construction, physics, action classes, and event processing
- Optical physics modeling (scintillation, absorption, boundary processes)
- ROOT files generated for each gamma energy
- A ROOT macro that computes efficiency curves and photon yield
- Example plots of the results

---

# 1. Detector Geometry

The detector is a **plastic scintillating fiber** with PMMA cladding.

### World
- Material: 'G4_AIR'
- Size: 10 cm × 10 cm × 10 cm

### Core (Scintillator)
- Material: 'G4_POLYSTYRENE'
- Length: 100 mm  
- Radius: 0.5 mm  
- Scintillation yield: 8000 photons/MeV  
- Decay constant: 2.7 ns  
- Refractive index: 1.60  
- Absorption length: 3.5 m  
- Emission around 2.4–2.6 eV

### Cladding (PMMA)
- Material: 'G4_PLEXIGLASS'
- Thickness: 0.03 mm  
- Refractive index: 1.49  

The scintillating core is made a **sensitive detector**, recording deposited energy, generated photons, and escaped photons.

---

# 2. Physics List

The simulation uses:

### ✔ G4EmStandardPhysics  
Provides gamma and charged particle interactions (photoelectric, Compton, Rayleigh, ionization,…)

### ✔ G4OpticalPhysics  
Includes scintillation, optical photon tracking, refraction, reflection, absorption.

### Production cuts
The simulation uses a range cuts of:
- 0.1 mm for γ, e⁻, e⁺  
(Consistent with the Tang paper.)

---

# 3. Input Gamma Energies

 -The simulation runs for the following monoenergetic gamma rays (in keV): 20, 30, 40, 50, 60, 80, 100, 150, 200, 300, 400, 500, 
 600, 700, 800, 900, 1000, 1200, 1400, 1600, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000

 -For each energy index 'i', the simulation produces a outputi.root stored in 'results' folder

 ---

# 4. ROOT macro
 -The ROOT macro efficiency_analysis.C present in the macros/ folder computes the Absorption efficiency (T_a), Optical Transmission efficiency (T_cT_t),
  Detection efficiency (DE), Photon yeild vs deposited energy. This macro also generates all the required outpul plots which are present in 'output plots' folder

 -T_a = <E_dep> / Energy of incident gamma
 -T_cT_t = <no. of eascaped optical photons> / <E_dep>
 -DE = <no. of eascaped photons> x 2.5 x 10^(-3) / energy of incident gamma  ( 2.5 eV is the energy of the generated optical photon)
 
---

# How to run the simulation
 -First make a build directory
 -Inside build directory run the following commands 
  ```bash
     cmake ..
     make -j4
     ./sim run.mac
  ``` 
 -All the output*.root files will be generated 
 -We have to run the root macro named "efficiency_analysis.C" which is present in the macros folder which will give all the required plots.
 ( it is important to run the macro inside build folder as all the root output files will be generated in build)
