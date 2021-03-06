# -*- ruby -*-

repository_base_url = "https://ruby-gnome2.svn.sourceforge.net/svnroot/ruby-gnome2/ruby-gnome2"

task :default => :build

def version
  @version ||= ENV["VERSION"] || guess_version
end

def guess_version
  versions = {}
  File.open("glib2/ext/glib2/rbglib.h") do |rbglib_h|
    rbglib_h.each_line do |line|
      if /#define\s+RBGLIB_([A-Z]+)_VERSION\s+(\d+)/ =~ line
        versions[$1.downcase] = $2.to_i
      end
    end
  end
  ["major", "minor", "micro"].collect {|type| versions[type]}.compact.join(".")
end

def dist_dir(base_name, _version=nil)
  _version ||= version
  "#{base_name}-#{_version}"
end

def archive_name(base_name, _version=nil)
  dist_dir(base_name, _version) + ".tar.gz"
end

@needless_paths = [".svn", ".test-result", ".gdb_history", "mkmf.log",
                   "*.o", "*.so", "pkg", "vendor"]
def package(base_name, paths, needless_paths=nil)
  _dist_dir = dist_dir(base_name)

  rm_rf(_dist_dir)
  mkdir_p(_dist_dir)
  cp_r(paths, _dist_dir, :preserve => true)

  needless_paths ||= @needless_paths
  Dir.glob("#{_dist_dir}/**/{#{needless_paths.join(',')}}") do |needless_path|
    rm_rf(needless_path)
  end
  sh("tar", "cvfz", archive_name(base_name), _dist_dir)
ensure
  rm_rf(_dist_dir) if _dist_dir
end

desc "configure all packages"
task :configure do
  ruby("extconf.rb")
end

file "Makefile" do
  task(:configure).invoke
end

desc "build all packages"
task :build => ["Makefile"] do
  sh("make")
end

desc "clean all packages"
task :clean do
  sh("make", "clean") if File.exist?("Makefile")
end

desc "more clean all packages"
task :distclean do
  sh("make", "distclean") if File.exist?("Makefile")
end

desc "run tests for all packages"
task :test => [:build] do
  ruby("run-test.rb")
end

desc "make all packages"
task :dist => [:dist_gtk2, :dist_gnome2]

base_files = ["AUTHORS", "COPYING.LIB", "ChangeLog", "NEWS",
              "README", "Rakefile",
              "exec_make.rb", "extconf.rb", "run-test.rb"]
gtk2_packages = ["glib2", "gio2", "atk", "pango", "gdk_pixbuf2", "gtk2"]
gtk2_base_name = "ruby-gtk2"
desc "make Ruby/GTK2 package"
task :dist_gtk2 do
  package(gtk2_base_name, base_files + gtk2_packages)
end

gnome2_packages = gtk2_packages + ["goocanvas", "gstreamer",
                                   "gtksourceview2", "poppler", "rsvg2", "vte"]
gnome2_base_name = "ruby-gnome2-all"
desc "make Ruby/GNOME2 package"
task :dist_gnome2 do
  package(gnome2_base_name, base_files + gnome2_packages)
end

desc "releae Ruby-GNOME2 packages"
task :release => [:dist] do
  sf_user_name = ENV["SVN_USER"] || ENV["USER"]
  project_id = "ruby-gnome2"
  project_name = "Ruby-GNOME 2"
  package_name = "ruby-gnome2"
  release_name = "ruby-gnome2-#{version}"
  archive_names = [gtk2_base_name, gnome2_base_name].collect do |base_name|
    archive_name(base_name)
  end
  ruby("misc/release.rb", sf_user_name, project_id, project_name,
       package_name, release_name, "README:1", "NEWS",
       *archive_names)
end

namespace :gem do
  desc "build all gems"
  task :build do
    gnome2_packages.each do |package|
      Dir.chdir(package) do
        ruby("-S", "rake", "gem")
      end
    end
  end

  desc "push all gems"
  task :push do
    gnome2_packages.each do |package|
      ruby("-S", "gem", "push",
           *Dir.glob(File.join(package, "pkg", "*-#{version}.gem")))
    end
  end

  desc "install all gems"
  task :install do
    gnome2_packages.each do |package|
      ruby("-S", "gem", "install", "--user-install",
           *Dir.glob(File.join(package, "pkg", "*-#{version}.gem")))
    end
  end

  desc "uninstall all gems"
  task :uninstall do
    gnome2_packages.each do |package|
      ruby("-S", "gem", "uninstall", "--version", version, package)
    end
  end

  namespace :win32 do
    win32_gnome2_packages = gnome2_packages - ["vte"]
    desc "build all Windows gems"
    task :build do
      win32_gnome2_packages.each do |package|
        Dir.chdir(package) do
          ruby("-S", "rake",
               "cross", "native", "gem", "RUBY_CC_VERSION=1.8.7:1.9.2")
        end
      end
    end

    desc "clean all Windows gems build"
    task :clean do
      win32_gnome2_packages.each do |package|
        rm_rf(File.join(package, "tmp"))
      end
    end

    desc "download DLL for Windows all gems"
    task :download do
      win32_gnome2_packages.each do |package|
        Dir.chdir(package) do
          ruby("-S", "rake", "win32:download")
        end
      end
    end

    desc "push all Windows gems"
    task :push do
      win32_gnome2_packages.each do |package|
        ruby("-S", "gem", "push",
             *Dir.glob(File.join(package, "pkg", "*-#{version}-x86-mingw32.gem")))
      end
    end
  end
end

def guess_copy_source_repository_uri
  info_xml = `svn info --xml`
  if /<url>(.+?)<\/url>/ =~ info_xml
    $1
  else
    raise "can't find repository URI from:\n#{info_xml}"
  end
end

desc "tag the current release"
task :tag do
  tagged_url = "#{repository_base_url}/tags/#{version}"

  success = false
  begin
    sh("svn", "ls", tagged_url)
    success = true
  rescue RuntimeError
  end
  raise "#{version} is already tagged" if success

  svn_commands = ["svn", "cp", "-m", "released #{version}!!!"]
  if ENV["SVN_USER"]
    svn_commands.concat(["--username", ENV["SVN_USER"]])
  end
  svn_commands.concat([guess_copy_source_repository_uri, tagged_url])
  sh(*svn_commands)
end
