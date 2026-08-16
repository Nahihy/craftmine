{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    nvf.url = "github:notashelf/nvf";
    import-tree.url = "github:vic/import-tree";
  };

  outputs = inputs: let
    pkgs = inputs.nixpkgs.legacyPackages.x86_64-linux;

    neovim = inputs.nvf.lib.neovimConfiguration {
      inherit pkgs;
      modules = [
        { _module.args = { nvf = inputs.nvf; }; }
        (inputs.import-tree ./flakeAssets/nvim)
      ];
    };

  in {
    devShells.x86_64-linux.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        clang
        clang-tools
        cmake
        libGL
        doxygen
        pkg-config
        wayland
        wayland-scanner
        wayland-protocols
        libxkbcommon
        libX11
        libffi
        libxcb
        libXrandr
        libXinerama
        libXcursor
        libXi
        libXxf86vm
        zlib
        assimp
        glfw
        neovim.neovim
      ];

      shellHook = ''
        PS1='\[\e[38;5;141;1m\][\[\e[38;5;111m\]dev-shell\[\e[38;5;112m\]@\[\e[38;5;111m\]nvim\[\e[38;5;112m\]:\[\e[38;5;219m\]\w\[\e[38;5;141m\]]\[\e[38;5;177m\]\\$\[\e[0m\] '
        echo 'This dev shell comes with custom neovim install. To use it run "nvim"'
        
        # Set up library paths for runtime
        export LD_LIBRARY_PATH="${pkgs.lib.makeLibraryPath [
          pkgs.libGL
          pkgs.wayland
          pkgs.libxkbcommon
          pkgs.libX11
          pkgs.libxcb
          pkgs.libXrandr
          pkgs.libXinerama
          pkgs.libXcursor
          pkgs.libXi
        ]}:$LD_LIBRARY_PATH"
        
        export PKG_CONFIG_PATH="${pkgs.wayland.dev}/lib/pkgconfig:${pkgs.libxkbcommon.dev}/lib/pkgconfig:$PKG_CONFIG_PATH"
        
        # Ensure XDG_RUNTIME_DIR is set for Wayland
        if [ -z "$XDG_RUNTIME_DIR" ]; then
          export XDG_RUNTIME_DIR=/run/user/$(id -u)
        fi
      '';
    };
  };
}





