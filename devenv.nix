{ pkgs, lib, config, inputs, ... }:
{
  packages = with pkgs; [
    git
  ];

  languages.python = {
    enable = true;
    venv = {
      enable = true;
      requirements = ''
        conan>=2.25,<3
      '';
    };
  };
}
