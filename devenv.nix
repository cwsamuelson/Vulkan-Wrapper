{ pkgs, lib, config, inputs, ... }:
{
  env = {
    CONAN_HOME = "${config.devenv.root}/.conan";
  };

  packages = with pkgs; [
    git

    gcc
    ninja
  ];

  languages.python = {
    enable = true;
    venv = {
      enable = true;
      requirements = ''
        conan>=2.25,<3
        spdx_license_list
      '';
    };
  };

  scripts = {
    build.exec = ''
      conan build ${config.devenv.root} $@
    '';

    clean.exec = ''
      rm -rf ${config.devenv.root}/build $@
    '';
  };
}
