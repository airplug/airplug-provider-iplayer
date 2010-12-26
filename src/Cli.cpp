#include "Downloader.h"
#include "Cli.h" // Header file for this implementation file

namespace IPDL {

  Cli::Cli(Downloader &downloader) {
    this->downloader   = downloader;
    this->succeeded    = true;
    this->quiet        = false;
  }

  Cli::Cli() : this(new Downloader()) {}

  void Cli::Run(string[] args) {
    bool showHelp    = false;
    bool showVersion = false;

    var opts = new OptionSet(){
      {"d=|download-path=", v => Directory.SetCurrentDirectory(v)},
      {"v|version",         v => showVersion = true},
      {"q|quiet",           v => quiet = true},
      {"h|help",            v => showHelp = true}
    };

    var identifiers = opts.Parse(args);

    if (showVersion) {
      ShowVersion();
      return;
    }

    if (showHelp || identifiers.Count == 0) {
      ShowHelp();
      return;
    }

    foreach (var identifier in identifiers) {
      Download(identifier);
    }
  }


  bool Cli::get_succeeded() {
    return this.succeeded;
  }

  void Cli::ShowVersion() {
    var details = Assembly.GetName();
    cout << details.Name << " version " << details.Version << endl;
  }

  void Cli::ShowHelp() {
    Console.WriteLine((new StreamReader(Assembly.GetManifestResourceStream("help.txt"))).ReadToEnd());
  }

  void Cli::Download(string identifier) {
    var pid = Util.ExtractPid(identifier);
    // Get the pid from the identifier

    if (pid == null) {
      if (!this.quiet)
	cout << "ERROR: " << identifier << " is not recognised as a programme ID" << endl;
      
      this.succeeded = false;
      return;
    }

    downloader.Download(pid, DownloadStart, DownloadProgress, DownloadEnd);
  }

  void Cli::DownloadStart(string filename) {
    if (this.quiet) return;

    cout << "Downloading: " << filename << endl;
  }

  void Cli::DownloadProgress(int bytesDownloaded, int total) {
    if (this.quiet) return;

    char *output = sprintf("%f%% of %f",
			    total,
			    (bytesDownloaded * 100.0) / total
			    );
				  
    if (strcmp(output, lastOutput)) {
      cout << output << endl;
      lastOutput = output;
    }
  }

  void Cli::DownloadEnd(DownloadStatus status, string message) {
    if (this.quiet)
      return;

    switch (status) {
    case DownloadStatus.Complete:
      cout << "SUCCESS: Downloaded to " << message;
      break;

    case DownloadStatus.Incomplete:
      cout << "FAILED: Incomplete download saved as " << message;
      this.succeeded = false;
      break;

    case DownloadStatus.AlreadyExists:
      cout << "SKIP: File exists: " << message;
      break;

    case DownloadStatus.Unavailable:
      cout << "ERROR: " << message << " is currently unavailable";
      this.succeeded = false;
      break;
    }

    cout << endl;
  }
}
