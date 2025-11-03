from convert_tlmcmddb2design_tlm import convert_tlmcmddb2design_tlm
from pathlib import Path

if __name__ == "__main__":
    # convert all tlms in tlm-cmd-db/TLM_DB/SAMPLE_MOBC_TLM_DB_*.csv
    tlm_path = Path(__file__).parent.parent.parent / "tlm-cmd-db" / "TLM_DB"
    out_dir = Path(__file__).parent.parent.parent / "design" / "tlm"
    out_dir.mkdir(parents=True, exist_ok=True)

    prefix = "AOBC_TLM_DB_"

    for tlm_file in sorted(tlm_path.glob("*.csv")):
        name = tlm_file.name
        out_name = name[len(prefix):] if name.startswith(prefix) else name

        out_file = out_dir / out_name
        print(f"Converting {tlm_file} -> {out_file} ...")
        convert_tlmcmddb2design_tlm(tlm_file, out_file)
