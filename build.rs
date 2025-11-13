fn main() {
    let mut c2a_cmake = cmake::Config::new(".");
    let libc2a = c2a_cmake
        .very_verbose(true)
        .define("C2A_BUILD_FOR_32BIT", "OFF")
        .define("C2A_BUILD_AS_C99", "ON")
        .define("C2A_USE_SCI_COM_WINGS", "OFF")
        .build_target("C2A");

    let target = std::env::var("TARGET").unwrap();
    let libc2a = if target == "thumbv7em-none-eabihf" {
        libc2a
            .target(&target)
            .define("CMAKE_TOOLCHAIN_FILE", "toolchain-monazite.cmake")
            .define("C2A_BUILD_FOR_SILS", "OFF")
            .define("C2A_USE_SIMPLE_LIBC", "ON")
    } else {
        libc2a.define("C2A_BUILD_FOR_SILS", "ON")
    };

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