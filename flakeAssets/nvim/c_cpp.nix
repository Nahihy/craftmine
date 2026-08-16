{ pkgs , lib , nvf , ... }: {


vim = {





   luaConfigRC.alpha-custom = lib.mkForce (lib.nvim.dag.entryAfter ["pluginConfigs"] ''
     local alpha = require("alpha")
     local dashboard = require("alpha.themes.dashboard")
  
     dashboard.section.header.val = vim.split([[
   
       ██████╗    ██╗ ██████╗██████╗ ██████╗ ██╗   ██╗██╗███╗   ███╗
      ██╔════╝   ██╔╝██╔════╝██╔══██╗██╔══██╗██║   ██║██║████╗ ████║
      ██║       ██╔╝ ██║     ██████╔╝██████╔╝██║   ██║██║██╔████╔██║
      ██║      ██╔╝  ██║     ██╔═══╝ ██╔═══╝ ╚██╗ ██╔╝██║██║╚██╔╝██║
      ╚██████╗██╔╝   ╚██████╗██║     ██║      ╚████╔╝ ██║██║ ╚═╝ ██║
       ╚═════╝╚═╝     ╚═════╝╚═╝     ╚═╝       ╚═══╝  ╚═╝╚═╝     ╚═╝
                                                                  
     ]], "\n")
 
     alpha.setup(dashboard.opts)
    '');

    keymaps = [
      { mode = "n"; key = "<leader>xg"; action = "<cmd>CMakeGenerate<cr>";        desc = "CMake: Configure"; }
      { mode = "n"; key = "<leader>xb"; action = "<cmd>CMakeBuild<cr>";           desc = "CMake: Build"; }
      { mode = "n"; key = "<leader>xr"; action = "<cmd>CMakeRun<cr>";             desc = "CMake: Run"; }
      { mode = "n"; key = "<leader>xd"; action = "<cmd>CMakeDebug<cr>";           desc = "CMake: Debug"; }
      { mode = "n"; key = "<leader>xt"; action = "<cmd>CMakeSelectBuildType<cr>"; desc = "CMake: Build Type"; }
      { mode = "n"; key = "<F5>";       action = "<cmd>lua require('dap').continue()<cr>";          desc = "Debug: Continue"; }
      { mode = "n"; key = "<F10>";      action = "<cmd>lua require('dap').step_over()<cr>";         desc = "Debug: Step Over"; }
      { mode = "n"; key = "<F11>";      action = "<cmd>lua require('dap').step_into()<cr>";         desc = "Debug: Step Into"; }
      { mode = "n"; key = "<F12>";      action = "<cmd>lua require('dap').step_out()<cr>";          desc = "Debug: Step Out"; }
      { mode = "n"; key = "<leader>db"; action = "<cmd>lua require('dap').toggle_breakpoint()<cr>"; desc = "Debug: Breakpoint"; }
      { mode = "n"; key = "<leader>du"; action = "<cmd>lua require('dapui').toggle()<cr>";          desc = "Debug: Toggle UI"; }
    ];

    languages.clang = {
      enable = true;
      lsp.enable = true;
      lsp.servers = [ "clangd" ];
      dap.enable = true;
      treesitter.enable = true;
    };

    debugger.nvim-dap = {
      enable = true;
      ui.enable = true;
    };

    luaConfigRC.dapui-close-fix = lib.nvim.dag.entryAfter ["pluginConfigs"] ''
      local dap, dapui = require("dap"), require("dapui")
      dap.listeners.before.event_terminated["dapui_config"] = function()
        dapui.close()
        vim.cmd("NvimTreeClose")
        vim.cmd("NvimTreeOpen")
       end
      dap.listeners.before.event_exited["dapui_config"] = function()
        dapui.close()
        vim.cmd("NvimTreeClose")
        vim.cmd("NvimTreeOpen")
      end
    '';

    languages.enableDAP = true;

    languages.cmake = {
      enable = true;
      lsp.enable = true;
      format.enable = true;
      treesitter.enable = true;
    };

    luaConfigRC.codelldb-setup = lib.nvim.dag.entryAfter ["pluginConfigs"] ''
      local dap = require("dap")
      dap.adapters.codelldb = {
        type = "server",
        port = "''${port}",
        executable = {
          command = "${pkgs.vscode-extensions.vadimcn.vscode-lldb}/share/vscode/extensions/vadimcn.vscode-lldb/adapter/codelldb",
          args = { "--port", "''${port}" },
        },
      }
    '';
    extraPlugins = {
      nvim-dap-virtual-text = {
        package = pkgs.vimPlugins.nvim-dap-virtual-text;
        after = ["nvim-dap"];
        setup = "require('nvim-dap-virtual-text').setup()";
      };
    };

    extraPackages = [ pkgs.vscode-extensions.vadimcn.vscode-lldb ];

    extraPlugins.clangd-extensions = {
      package = pkgs.vimPlugins.clangd_extensions-nvim;
      after = ["nvim-lspconfig"];
      setup = "require('clangd_extensions').setup()";
    };

    extraPlugins = {
      cmake-tools = {
        package = pkgs.vimPlugins.cmake-tools-nvim;
        after = ["nvim-dap"];
        setup = ''
          require("cmake-tools").setup {
            cmake_build_directory = "build",
            cmake_generate_options = { "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON" },
            cmake_soft_link_compile_commands = true,
          }
        '';
      };
    };


  };




}
