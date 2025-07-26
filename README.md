# masters-arm-datapath
Master's Project: "Modeling the Execution Engine (Datapath) Portion of an ARM Processor"

📅 **Degree**: M.S. in Electrical and Computer Engineering  
🏫 **Institution**: University at Albany, SUNY  
👨‍🏫 **Advisor**: Professor James R. Moulic  
🎯 **Goal**: Simulate a simplified LEGv8 5-stage pipelined processor and convert the model into synthesizable VHDL

---

## 🛠️ Project Description

This project models and simulates a simplified ARMv8-style processor using a subset of the LEGv8 instruction set. The design focuses on the **execution engine (datapath)**, which consists of:

- Instruction Fetch (IF)
- Instruction Decode (ID)
- Operand Fetch (OF)
- Execute (EX)
- Write Back (WB)

---

## ⚙️ Technologies Used

- **C Programming** — simulation of the pipeline  
- **Vitis HLS** — synthesis of C into VHDL  
- **Vivado** — RTL simulation and waveform analysis  
- **LEGv8 ISA** — instruction set architecture modeled after ARMv8  

---

## 💻 Features Implemented

- Simulated instructions: `ADD`, `SUB`, `ADDI`, `SUBI`, `AND`, `ORR`, `LDUR`, `STUR`  
- Register file and main memory modeled as 2D arrays  
- Opcode decoding and operand extraction  
- Testbench for waveform analysis using Vivado  

---

## 🧪 Simulation Results

- ✅ Verified 5-stage execution of each instruction  
- ✅ Correct memory and register interactions  
- ✅ RTL waveforms confirm proper operation (see screenshot)

![Waveform Example](assets/waveform.png)

---

## 📂 File/Folder Structure

```
├── C-Code
├── Vivado
├── MP_Final_Report.pdf
├── README.md
```

---

## 📥 Downloads

- 📘 [Final Report (PDF)](MP_Final_Report.pdf)  
- 🌐 [Portfolio Website](https://drsarojshah.github.io)  

---

## 📫 Contact

- 📧 Email: sarojshah381@yahoo.com  
- 🔗 LinkedIn: [linkedin.com/in/saroj-s-763265226](https://linkedin.com/in/saroj-s-763265226)

---

> ⚠️ **Note**: This is a simplified simulation model and may require optimization before FPGA deployment on constrained platforms like Basys3.
