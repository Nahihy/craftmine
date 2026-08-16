{ pkgs , ... }: {
  vim = {


    treesitter.grammars = with pkgs.vimPlugins.nvim-treesitter.grammarPlugins; [
      glsl
    ];



  };
}
