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
		Console.WriteLine("{0} version {1}", details.Name, details.Version);
	}

	void Cli::ShowHelp() {
		Console.WriteLine((new StreamReader(Assembly.GetManifestResourceStream("help.txt"))).ReadToEnd());
	}

	Assembly Cli::get_Assembly() {
		return Assembly.GetExecutingAssembly();
	}

	void Cli::Download(string identifier) {
		var pid = Util.ExtractPid(identifier);
		if (pid == null) {
			if (!this.quiet) {
				Console.WriteLine("ERROR: {0} is not recognised as a programme ID", identifier);
			}
			this.succeeded = false;
			return;
		}
		downloader.Download(pid, DownloadStart, DownloadProgress, DownloadEnd);
	}

	void Cli::DownloadStart(string filename) {
		if (this.quiet) return;

		Console.WriteLine("Downloading: {0}", filename);
	}

	void Cli::DownloadProgress(int bytesDownloaded, int total) {
		if (this.quiet) return;

		string output = String.Format("{1:0.0}% of {0}",
																	Util.SIFormat(total, "B"),
																	(bytesDownloaded * 100.0) / total);
		if (output != lastOutput) {
#if !TEST_BUILD
			Console.CursorLeft = 0;
#endif
			Console.Write(output);
			lastOutput = output;
		}
	}

	void Cli::DownloadEnd(DownloadStatus status, string message) {
		string output = "";
		switch (status) {
		case DownloadStatus.Complete:
			output = String.Format("SUCCESS: Downloaded to {0}", message);
			break;
		case DownloadStatus.Incomplete:
			output = String.Format("FAILED: Incomplete download saved as {0}", message);
			this.succeeded = false;
			break;
		case DownloadStatus.AlreadyExists:
			output = String.Format("SKIP: File exists: {0}", message);
			break;
		case DownloadStatus.Unavailable:
			output = String.Format("ERROR: {0} is currently unavailable", message);
			this.succeeded = false;
			break;
		}
		if (!this.quiet) {
			Console.WriteLine();
			Console.WriteLine(output);
		}
	}
}
