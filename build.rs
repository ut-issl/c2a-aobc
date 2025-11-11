fn main() {
    let mut c2a_cmake = cmake::Config::new(".");
    let libc2a = c2a_cmake
        .very_verbose(true)
        .define("C2A_BUILD_FOR_32BIT", "OFF")
        .define("C2A_BUILD_AS_C99", "ON")
        .define("C2A_USE_SCI_COM_WINGS", "OFF")
        .define("C2A_SHOW_DEBUG_PRINT_ON_SILS", "ON")
        .build_target("C2A");

    // Build C2A & link
    println!("cargo:rerun-if-changed=./src/src_core");
    println!("cargo:rerun-if-changed=./src/src_user");

    let libc2a = libc2a.build();
    println!(
        "cargo:rustc-link-search=native={}/build", // no install step in libC2A
        libc2a.display()
    );
    println!("cargo:rustc-link-lib=static=C2A");
}