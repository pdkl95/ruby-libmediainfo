require 'spec_helper'
require 'libmediainfo'
require 'libmediainfo/version'

describe Libmediainfo do
  describe "Libmediainfo::VERSION" do
    subject { Libmediainfo::VERSION }
    it { should_not be_empty }
    describe "Libmediainfo::VERSION::MAJOR" do
      subject { Libmediainfo::VERSION::MAJOR }
      it { should be_instance_of(Fixnum) }
      it { should >= 0 }
    end
    describe "Libmediainfo::VERSION::MINOR" do
      subject { Libmediainfo::VERSION::MINOR }
      it { should be_instance_of(Fixnum) }
      it { should >= 0 }
    end
    describe "Libmediainfo::VERSION::PATCH" do
      subject { Libmediainfo::VERSION::PATCH }
      it { should be_instance_of(Fixnum) }
      it { should >= 0 }
    end
    describe "Libmediainfo::VERSION::STRING" do
      subject { Libmediainfo::VERSION::STRING }
      it { should be_instance_of(String) }
      it { should match(/^\d+\.\d+\.\d+$/) }
    end
  end
end
