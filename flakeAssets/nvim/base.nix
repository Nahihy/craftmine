{ pkgs , lib , nvf , ... }: {


  vim = {
    viAlias = false;
    vimAlias = false;

    options = {
      tabstop = 2;
      shiftwidth = 2;    # match tabstop
      expandtab = true;  # spaces instead of tabs
      smartindent = true;
    };


    # Clipboard
    clipboard = {
      enable = true;
      registers = "unnamedplus";
      providers.wl-copy.enable = true;
    };

     # Theme
    theme = {
      enable = true;
      name = "catppuccin";
      style = "mocha";
    };

    
    utility.images.image-nvim = {
      enable = true; 
      setupOpts.backend = "sixel";
    };



    luaConfigRC.alpha-custom = lib.nvim.dag.entryAfter ["pluginConfigs"] ''
      local alpha = require("alpha")
      local dashboard = require("alpha.themes.dashboard")
    
      dashboard.section.header.val = vim.split([[
     
        ███╗   ██╗███████╗ ██████╗ ██╗   ██╗██╗███╗   ███╗    
        ████╗  ██║██╔════╝██╔═══██╗██║   ██║██║████╗ ████║    
        ██╔██╗ ██║█████╗  ██║   ██║██║   ██║██║██╔████╔██║    
        ██║╚██╗██║██╔══╝  ██║   ██║╚██╗ ██╔╝██║██║╚██╔╝██║    
        ██║ ╚████║███████╗╚██████╔╝ ╚████╔╝ ██║██║ ╚═╝ ██║    
        ╚═╝  ╚═══╝╚══════╝ ╚═════╝   ╚═══╝  ╚═╝╚═╝     ╚═╝    

      ]], "\n")

      alpha.setup(dashboard.opts)
   '';

    # UI
    statusline.lualine.enable = true;
    tabline.nvimBufferline.enable = true;
    telescope.enable = true;

    filetree.nvimTree = {
      enable = true;
      openOnSetup = true;
      setupOpts = {
        actions.open_file.resize_window = true;
        view = {
          width = {
              min = 25;
              max = 30;
              padding = 1;
          };
          preserve_window_proportions = true;
        };
      };
    };

    # Breadcrumbs (shows where you are in code, e.g. class > function)
    ui.breadcrumbs = {
      enable = true;
      navbuddy.enable = true;  # popup to jump to symbols
    };

    

    # LSP diagnostics UI
    ui.noice.enable = true;        # nicer cmdline/notifications
    visuals.fidget-nvim.enable = true;  # LSP loading indicator in corner

    # Treesitter
    treesitter = {
      enable = true;
      context.enable = true;  # sticky header showing current function/block
    };

    lsp.enable = true;


    # Languages
    languages = {
     enableFormat = true;
      enableTreesitter = true;
      enableExtraDiagnostics = true;

      nix = {
        enable = true;
        lsp.servers = [ "nixd" ];
        format.type = [ "alejandra" ];
      };
    };

    # Completion
    autocomplete.nvim-cmp.enable = true;
    snippets.luasnip.enable = true;

    # Editing helpers
    autopairs.nvim-autopairs.enable = true;
    comments.comment-nvim.enable = true;
    visuals.indent-blankline.enable = true;       # indent guides
    visuals.nvim-cursorline.enable = true; # highlight current word/line

  # Terminal
    terminal.toggleterm = {
      enable = true;
      lazygit.enable = true;  # git TUI built in
      mappings.open = "<leader>w";

      setupOpts = {
        direction = "float";  

      };
    };

    

    # Dashboard on open
    dashboard.alpha.enable = true;

    # Keybind helper (press space and see all keybinds)
    binds.whichKey.enable = true;

    # Session management
    session.nvim-session-manager = {
      enable = true;
      setupOpts.autoload_mode = "Disabled";
    };

    # Notifications
    notify.nvim-notify.enable = true;
  };

}
